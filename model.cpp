#include <vector>
#include <string>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader\tiny_obj_loader.h"
#include "glm/mat4x4.hpp"
#include "gl_core_4_3.h"

#include "model.h"
#include "Graphic.h"
#include "ext_glm.h"
#include "Resources.h"

namespace
{

	inline void ReverseTextureCoord(std::vector<float>& texcoord)
	{
		for (unsigned i = 0; i < texcoord.size(); i+=2)
		{
			texcoord[i+1] = std::ceil(texcoord[i+1]) - texcoord[i+1];
		}
	}

	void ConvertToModelFormat(Model& m, std::vector<tinyobj::shape_t>&& shapes)
	{
		m.ClearData();
		int numPositions(0);

		for (auto& shape : shapes)
		{
			numPositions += shape.mesh.positions.size();
		}

		m.GetVertices().reserve(numPositions);
		m.GetNormals().reserve(numPositions);
		m.GetTexCoords().reserve(numPositions);
		m.GetIndices().reserve(numPositions / 3);

		for (auto& shape: shapes)
		{
			m.GetVertices().insert(m.GetVertices().end(), shape.mesh.positions.begin(), shape.mesh.positions.end());
			m.GetNormals().insert(m.GetNormals().end(), shape.mesh.normals.begin(), shape.mesh.normals.end());
			m.GetTexCoords().insert(m.GetTexCoords().end(), shape.mesh.texcoords.begin(), shape.mesh.texcoords.end());
			m.GetIndices().insert(m.GetIndices().end(), shape.mesh.indices.begin(), shape.mesh.indices.end());
		}

		m.GetTexCoords().shrink_to_fit();
		m.GetVertices().shrink_to_fit();
		m.GetNormals().shrink_to_fit();
		m.GetIndices().shrink_to_fit();

		ReverseTextureCoord(m.GetTexCoords());
	}
}

Model::Model() :
transformMat(ext_glm::IdentityMat()),
position(0, 0, 0, 1),
slopeAngle(0),
scale(1),
type(Model::Type::commonModel),
vertexArrayBuffer(0),
indicesBuffer(0),
normalsBuffer(0),
texturecoordsBuffer(0)
{};

int Model::LoadModel(const char* filename)
{
	assert(filename);
	std::string err;
	std::vector<tinyobj::material_t> materials;
	std::vector<tinyobj::shape_t> shapes;

	if (!tinyobj::LoadObj(shapes, materials, err, filename))
	{
		std::cout << err.c_str();
		return 1;
	} 

	ConvertToModelFormat(*this, std::move(shapes));
	SetBoundingBox();
	ScaleToBoundingBox();
	CalculateTransformMat();
	MoveToCenter();
	CalculateTransformMat();
	return 0;
}

void Model::ClearData(bool freeMemory) noexcept
{
	vertices.clear();
	indices.clear();
	normals.clear();
	texturecoords.clear();

	if (freeMemory)
	{
		vertices.shrink_to_fit();
		indices.shrink_to_fit();
		normals.shrink_to_fit();
		texturecoords.shrink_to_fit();
	}
}

void Model::LoadGlData()
{
	// Load all GL buffers, include textures buffers

	/* Work with vertex buffer array */
	glGenVertexArrays(1, &vertexArrayBuffer);
	glBindVertexArray(vertexArrayBuffer);

	glEnableVertexAttribArray(static_cast<GLint>(Graphic::VertexAtrib::VertexCoors));
	glEnableVertexAttribArray(static_cast<GLint>(Graphic::VertexAtrib::Normals));
	glEnableVertexAttribArray(static_cast<GLint>(Graphic::VertexAtrib::TextureCoord));

	if (verticesBuffer || normalsBuffer || texturecoordsBuffer)
		ClearGLBuffers();

	/* Work with vertex buffer array */
	glGenBuffers(1, &verticesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &texturecoordsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texturecoordsBuffer);
	glBufferData(GL_ARRAY_BUFFER, texturecoords.size() * sizeof(float), texturecoords.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glVertexAttribPointer(static_cast<GLint>(Graphic::VertexAtrib::VertexCoors), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glVertexAttribPointer(static_cast<GLint>(Graphic::VertexAtrib::Normals), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, texturecoordsBuffer);
	glVertexAttribPointer(static_cast<GLint>(Graphic::VertexAtrib::TextureCoord), 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(0);

	material.LoadTexuresGLBuffers(*this);
}

void Model::Draw(const Resources& resources)
{
	assert(std::fmod(indices.size(), 3) == 0);

	glBindVertexArray(vertexArrayBuffer);
	material.BindTextures();

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::mat4 Model::CalculateTransformMat()
{
	transformMat = ext_glm::move(position.x, position.y, position.z) * ext_glm::rotateXYZ(slopeAngle) * ext_glm::scale(scale);

	boundingBox[0] = transformMat * boundingBox[0];
	boundingBox[1] = transformMat * boundingBox[1];
	return transformMat;
}

void Model::SetBoundingBox()
{
	boundingBox[0] = glm::vec4(vertices[indices.front()], vertices[indices.front()+1], vertices[indices.front()+2], 1);
	boundingBox[1] = glm::vec4(vertices[indices.front()], vertices[indices.front() + 1], vertices[indices.front() + 2], 1);


	for (size_t i = 0; i < indices.size(); ++i)
	{
		int curInd = 3*indices[i];
		if (vertices[curInd] < boundingBox[0].x)
		{
			boundingBox[0].x = vertices[curInd];
		}
		if (vertices[curInd + 1] < boundingBox[0].y)
		{
			boundingBox[0].y = vertices[curInd + 1];
		}
		if (vertices[curInd + 2] < boundingBox[0].z)
		{
			boundingBox[0].z = vertices[curInd + 2];
		}

		if (vertices[curInd] > boundingBox[1].x)
			boundingBox[1].x = vertices[curInd];
		if (vertices[curInd + 1] > boundingBox[1].y)
			boundingBox[1].y = vertices[curInd + 1];
		if (vertices[curInd + 2] > boundingBox[1].z)
			boundingBox[1].z = vertices[curInd + 2];
	}
}

void Model::ScaleToBoundingBox()
{
	float diffX = std::fabs(boundingBox[0].x - boundingBox[1].x);
	float diffY = std::fabs(boundingBox[0].y - boundingBox[1].y);
	float diffZ = std::fabs(boundingBox[0].z - boundingBox[1].z);

	if (diffX > 2 || diffY > 2 || diffZ > 2)
	{
		float max = std::max({ diffX, diffY, diffZ });
		scale = 2 / max;
	}
}

void Model::MoveToCenter()
{
	float centerX = -1*(boundingBox[1].x + boundingBox[0].x)/2;
	float centerY =  -1*(boundingBox[1].y + boundingBox[0].y)/2;
	float centerZ = -1*(boundingBox[1].z + boundingBox[0].z)/2;

	position = glm::vec4(centerX, centerY, centerZ, 1);
}

void Model::LoadModelUniforms(const ShaderProgram& shaderProgram) const
{
	material.PassToShader(shaderProgram);
}