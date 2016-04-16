#pragma once

#include "glm\mat4x4.hpp"
#include "glm\vec4.hpp"
#include "glm\vec3.hpp"
#include "gl_core_4_3.h"

#include <array>
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
	glm::mat4 CalculateTransformMat();
	
	/* Data */
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> textures;
	std::vector<unsigned int> indices;

	// 0 - min, 1 - max
	std::array<glm::vec4, 2> boundingBox;
	glm::vec4 position;
	glm::vec3 slopeAngle;
	unsigned int scale;

	/* Matrix */
	GLint transMatLoc;
	glm::mat4 transformMat;

	/* Buffers */
	GLuint vertexArrayBuffer;
	GLuint verticesBuffer;
	GLuint normalsBuffer;
	GLuint indicesBuffer;

private:
	void SetBoundingBox();
	void ScaleToBoundingBox();

};