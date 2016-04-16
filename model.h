#pragma once

#include "glm\mat4x4.hpp"
#include "gl_core_4_3.h"

#include <vector>

class Model
{
public:
	Model();
	~Model() = default;
	Model(const Model& m) = default;

	int LoadModel(const char* filename);
	void LoadGlData();
	void ClearData(bool freeMemory=false) noexcept;
	void Draw() const;
	
	/* Data */
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> textures;
	std::vector<unsigned int> indices;

	/* Matrix */
	GLint transMatLoc;
	glm::mat4 transformMat;

	/* Buffers */
	GLuint vertexArrayBuffer;
	GLuint verticesBuffer;
	GLuint normalsBuffer;
	GLuint indicesBuffer;
};