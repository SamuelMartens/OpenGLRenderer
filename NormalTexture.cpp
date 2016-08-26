#include "NormalTexture.h"

#include <iostream>
#include <unordered_map>
#include <cassert>

#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"

#include "model.h"
#include "Graphic.h"

namespace
{
	inline void InsertTangentInMap(std::unordered_map<unsigned, glm::vec3>& tangentsMap, std::pair<unsigned, glm::vec3>&& newTangent)
	{
		auto res = tangentsMap.insert(newTangent);

		if (res.second)
			return;

		res.first->second = res.first->second + newTangent.second;
		res.first->second.x /= 2;
		res.first->second.y /= 2;
		res.first->second.z /= 2; 
	}

	inline glm::mat3 CreateVerticesDiffMat(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& normal)
	{
		// Create matrix of vertices coords diff and normal for calculating tangents
		// WARNING: glm use column major layout
		const float floatMat[9] =
		{
			p1.x - p0.x, p1.y - p0.y, p1.z - p0.z,
			p2.x - p0.x, p2.y - p0.y, p2.z - p0.z,
			normal.x, normal.y, normal.z
		};

		return glm::make_mat3(floatMat);
	}

	inline glm::vec3 CreateTextureCoordDiffVec(const float u0, const float u1, const float u2)
	{
		return glm::vec3(u1 - u0, u2 - u0, 0);
	}
}


void NormalTexture::CalculateTangAndBitang(const Model& model)
{
	constexpr unsigned int coordsOfOneVertex = 3;
	constexpr unsigned int coordsOfOneText = 2;

	if (model.texturecoords.empty())
	{
		std::cout << "Tangens fot normal texture couldn't be calculated. \n";
		return;
	}

	std::unordered_map<unsigned, glm::vec3> tangentsMap;
	std::unordered_map<unsigned, glm::vec3> bitangetsMap;

	for (unsigned i = 0; i < model.indices.size(); i += coordsOfOneVertex)
	{
		const unsigned indP0 = model.indices[i] * coordsOfOneVertex;
		const unsigned indP1 = model.indices[i + 1] * coordsOfOneVertex;
		const unsigned indP2 = model.indices[i + 2] * coordsOfOneVertex;
		const unsigned indTP0 = model.indices[i] * coordsOfOneText;
		const unsigned indTP1 = model.indices[i + 1] * coordsOfOneText;
		const unsigned indTP2 = model.indices[i + 2] * coordsOfOneText;


		// Vertices of our triangle ( X Y Z )
		glm::vec3 p0(model.vertices[indP0], model.vertices[indP0 + 1], model.vertices[indP0 + 2]);
		glm::vec3 p1(model.vertices[indP1], model.vertices[indP1 + 1], model.vertices[indP1 + 2]);
		glm::vec3 p2(model.vertices[indP2], model.vertices[indP2 + 1], model.vertices[indP2 + 2]);

		// Texcoord of our triangle vertices ( U V )
		glm::vec2 texP0(model.texturecoords[indTP0], model.texturecoords[indTP0 + 1]);
		glm::vec2 texP1(model.texturecoords[indTP1], model.texturecoords[indTP1 + 1]);
		glm::vec2 texP2(model.texturecoords[indTP2], model.texturecoords[indTP2 + 1]);

		// Normals of our triangle ( X Y Z )
		glm::vec3 normP0(model.normals[indP0], model.normals[indP0 + 1], model.normals[indP0 + 2]);
		glm::vec3 normP1(model.normals[indP1], model.normals[indP1 + 1], model.normals[indP1 + 2]);
		glm::vec3 normP2(model.normals[indP2], model.normals[indP2 + 1], model.normals[indP2 + 2]);

		// Calculate tangent vec for p0
		glm::mat3 tempMat = glm::inverse(CreateVerticesDiffMat(p0, p1, p2, normP0));
		glm::vec3 tempVec = CreateTextureCoordDiffVec(texP0.y, texP1.y, texP2.y) * tempMat;
		InsertTangentInMap(tangentsMap, std::pair<unsigned, glm::vec3>(model.indices[i], tempVec));
		tempVec = CreateTextureCoordDiffVec(texP0.x, texP1.x, texP2.x) * tempMat;
		InsertTangentInMap(bitangetsMap, std::pair<unsigned, glm::vec3>(model.indices[i], tempVec));

		// Calculate tangent vec for p1
		tempMat = glm::inverse(CreateVerticesDiffMat(p1, p2, p0, normP1));
		tempVec = CreateTextureCoordDiffVec(texP1.y, texP2.y, texP0.y) * tempMat;
		InsertTangentInMap(tangentsMap, std::pair<unsigned, glm::vec3>(model.indices[i + 1], tempVec));
		tempVec = CreateTextureCoordDiffVec(texP1.x, texP2.x, texP0.x) * tempMat;
		InsertTangentInMap(bitangetsMap, std::pair<unsigned, glm::vec3>(model.indices[i + 1], tempVec));

		// Calculate tangent for p2
		tempMat = glm::inverse(CreateVerticesDiffMat(p2, p0, p1, normP2));
		tempVec = CreateTextureCoordDiffVec(texP2.y, texP0.y, texP1.y) * tempMat;
		InsertTangentInMap(tangentsMap, std::pair<unsigned, glm::vec3>(model.indices[i + 2], tempVec));
		tempVec = CreateTextureCoordDiffVec(texP2.x, texP0.x, texP1.x) * tempMat;
		InsertTangentInMap(bitangetsMap, std::pair<unsigned, glm::vec3>(model.indices[i + 2], tempVec));
	}

	tangents.clear();
	tangents.resize(model.normals.size());
	bitangets.clear();
	bitangets.resize(model.normals.size());

	// Fill our tangent and bitangents vectors
	assert(bitangetsMap.size() == tangentsMap.size());
	for (auto& t : tangentsMap)
	{
		tangents[t.first] = t.second.x;
		tangents[t.first + 1] = t.second.y;
		tangents[t.first + 2] = t.second.z;
	}
	for (auto& bt : bitangetsMap)
	{
		tangents[bt.first] = bt.second.x;
		tangents[bt.first + 1] = bt.second.y;
		tangents[bt.first + 2] = bt.second.z;
	}

}

void NormalTexture::LoadGLModelBuffer(const Model& model)
{
	assert(model.vertexArrayBuffer);

	glBindVertexArray(model.vertexArrayBuffer);
	glEnableVertexAttribArray(static_cast<GLint>(Graphic::VertexAtrib::Tangents));
	glEnableVertexAttribArray(static_cast<GLint>(Graphic::VertexAtrib::Bitangents));
	
	if (tangentsBuffer || bitangentsBuffer)
		CleanGLBuffers();

	glGenBuffers(1, &tangentsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tangentsBuffer);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(float), tangents.data(), GL_STATIC_DRAW);
	
	glGenBuffers(1, &bitangentsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentsBuffer);
	glBufferData(GL_ARRAY_BUFFER, bitangets.size() * sizeof(float), bitangets.data(), GL_STATIC_DRAW);
	// Perhaps I should Bind indices buffer here?
	glVertexAttribPointer(static_cast<GLint>(Graphic::VertexAtrib::Tangents), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(static_cast<GLint>(Graphic::VertexAtrib::Bitangents), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(0);
}

int NormalTexture::GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const
{
	int loc = glGetUniformLocation(shaderProgram.id, "material.normalTexture");

	if (-1 == loc)
		std::cout << "Failed to get diffuse texture uniform location \n";

	return loc;
}