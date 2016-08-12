#include "NormalTexture.h"

#include <iostream>
#include <unordered_map>
#include <cassert>

#include "glm\glm.hpp"

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
}


void NormalTexture::CalculateTangens(const Model& model)
{
	constexpr unsigned int coordsOfOneVertex = 3;

	if (model.texturecoords.empty())
	{
		std::cout << "Tangens fot normal texture couldn't be calculated. \n";
		return;
	}

	std::unordered_map<unsigned, glm::vec3> tangentsMap;
	
	for (unsigned i = 0; i < model.indices.size(); i += coordsOfOneVertex)
	{
		// Vertices of our triangle ( X Y Z )
		glm::vec3 p0(model.vertices[model.indices[i]], model.vertices[model.indices[i] + 1], model.vertices[model.indices[i] + 2]);
		glm::vec3 p1(model.vertices[model.indices[i + 1]], model.vertices[model.indices[i + 1] + 1], model.vertices[model.indices[i + 1] + 2]);
		glm::vec3 p2(model.vertices[model.indices[i + 2]], model.vertices[model.indices[i + 2] + 1], model.vertices[model.indices[i + 2] + 2]);

		// Texcoord of triangle vertices ( U V )
		glm::vec2 texP0(model.texturecoords[model.indices[i]], model.texturecoords[model.indices[i + 1]]);
		glm::vec2 texP1(model.texturecoords[model.indices[i + 1]], model.texturecoords[model.indices[i + 1] + 1]);
		glm::vec2 texP2(model.texturecoords[model.indices[i + 2]], model.texturecoords[model.indices[i + 2] + 1]);

		// Calculate tangent vec for p0
		glm::vec3 tangent = ((p2 - p0) - (p1 - p0) * (texP2.x - texP0.x) / (texP1.x - texP0.x)) /
			((texP2.y - texP0.y) - (texP1.y - texP0.y) * (texP2.x - texP0.x) / (texP1.x - texP0.x));
		InsertTangentInMap(tangentsMap, std::pair<unsigned, glm::vec3>(model.indices[i], tangent));

		// Calculate tangent vec for p1
		tangent = ((p0 - p1) - (p2 - p1) * (texP0.x - texP1.x) / (texP2.x - texP1.x)) /
			((texP0.y - texP1.y) - (texP2.y - texP1.y) * (texP0.x - texP1.x) / (texP2.x - texP1.x));
		InsertTangentInMap(tangentsMap, std::pair<unsigned, glm::vec3>(model.indices[i + 1], tangent));

		// Calculate tangent for p2
		tangent = ((p1 - p2) - (p0 - p2) * (texP1.x - texP2.x) / (texP0.x - texP2.x)) /
			((texP1.y - texP2.y) - (texP0.y - texP2.y) * (texP1.x - texP2.x) / (texP0.x - texP2.x));
		InsertTangentInMap(tangentsMap, std::pair<unsigned, glm::vec3>(model.indices[i + 2], tangent));
	}

	tangents.clear();
	tangents.resize(model.normals.size());

	// Fill our tangent vector
	for (auto t : tangentsMap)
	{
		tangents[t.first] = t.second.x;
		tangents[t.first + 1] = t.second.y;
		tangents[t.first + 2] = t.second.z;
	}
}

void NormalTexture::LoadTangensInGLModelBuffer(const Model& model)
{
	assert(model.vertexArrayBuffer);

	glBindVertexArray(model.vertexArrayBuffer);
	glEnableVertexAttribArray(static_cast<GLint>(Graphic::VertexAtrib::Tangents));
	
	if (tangentsBuffer)
		CleanGLBuffers();

	glGenBuffers(1, &tangentsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tangentsBuffer);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(float), tangents.data(), GL_STATIC_DRAW);
	// Perhaps I should Bind indices buffer here?
	glVertexAttribPointer(static_cast<GLint>(Graphic::VertexAtrib::Normals), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(0);
}

int NormalTexture::GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const
{
	int loc = glGetUniformLocation(shaderProgram.id, "material.normalTexture");

	if (-1 == loc)
		std::cout << "Failed to get diffuse texture uniform location \n";

	return loc;
}