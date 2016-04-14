#include "model.h"
#include "Graphic.h"

#include "tiny_obj_loader\tiny_obj_loader.h"
#include "gl_core_4_3.h"

#include <vector>
#include <string>
#include <cassert>
#include <cmath>

namespace
{
	void ConvertToModelFormat(Model& m, std::vector<tinyobj::shape_t>&& shapes)
	{
		m.ClearData();
		int numPositions(0);

		for (auto& shape : shapes)
		{
			numPositions += shape.mesh.positions.size();
		}

		m.vertices.reserve(numPositions);
		m.normals.reserve(numPositions);

		for (auto& shape: shapes)
		{
			m.vertices.insert(m.vertices.end(), shape.mesh.positions.begin(), shape.mesh.positions.end());
			m.normals.insert(m.normals.end(), shape.mesh.normals.begin(), shape.mesh.normals.end());
			m.indices.insert(m.indices.end(), shape.mesh.indices.begin(), shape.mesh.indices.end());
		}

		m.vertices.shrink_to_fit();
		m.normals.shrink_to_fit();
	}
}

int Model::LoadModel(const char* filename)
{
	std::string err;
	std::vector<tinyobj::material_t> materials;
	std::vector<tinyobj::shape_t> shapes;

	if (!tinyobj::LoadObj(shapes, materials, err, filename))
		return 1;

	ConvertToModelFormat(*this, std::move(shapes));
}

void Model::ClearData(bool freeMemory) noexcept
{
	vertices.clear();
	indices.clear();
	normals.clear();
	textures.clear();

	if (freeMemory)
	{
		vertices.shrink_to_fit();
		indices.shrink_to_fit();
		normals.shrink_to_fit();
		textures.shrink_to_fit();
	}
}

int Model::LoadGlData()
{
	/* Work with vertex buffer array */
	glGenBuffers(1, &verticesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

	/* Work with vertex buffer array */
	glGenVertexArrays(1, &vertexArrayBuffer);
	glBindVertexArray(vertexArrayBuffer);

	glEnableVertexAttribArray(static_cast<GLint>(Graphic::VertexAtrib::VertexCoors));
	glEnableVertexAttribArray(static_cast<GLint>(Graphic::VertexAtrib::Normals));

	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glVertexAttribPointer(static_cast<GLint>(Graphic::VertexAtrib::VertexCoors), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glVertexAttribPointer(static_cast<GLint>(Graphic::VertexAtrib::Normals), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//glBindVertexArray(vertexArrayBuffer);
}

void Model::Draw() const
{
	assert(std::fmod(indices.size(), 3) == 0);
	glDrawArrays(GL_TRIANGLES, 0, indices.size()/3);
}