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

	enum class Type
	{
		commonModel,
		lightModel
	};

	Model();
	Model(const Model& m) = default;
	~Model()
	{
		GLuint buffers[3] = { verticesBuffer, normalsBuffer, indicesBuffer };
		glDeleteVertexArrays(1, &vertexArrayBuffer);
		glDeleteBuffers(3, buffers);
	};

	int LoadModel(const char* filename);
	void LoadGlData();
	void LoadModelUniforms(GLuint shaderProgram);
	void ClearData(bool freeMemory=false) noexcept;
	void Draw() const;
	glm::mat4 CalculateTransformMat();
	void MoveToCenter();
	
	/* Data */
	Type type;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> textures;
	std::vector<unsigned int> indices;

	// 0 - min, 1 - max
	std::array<glm::vec4, 2> boundingBox;
	glm::vec4 position;
	glm::vec3 slopeAngle;
	float scale;

	/* Materials data */
	glm::vec3 Kd;
	glm::vec3 Ks;
	glm::vec3 Ka;

	/* Matrix */
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