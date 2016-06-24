#pragma once

#include <array>
#include <vector>

#include "glm\mat4x4.hpp"
#include "glm\vec4.hpp"
#include "glm\vec3.hpp"
#include "gl_core_4_3.h"

#include "Material.h"

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
	~Model() = default;

	int LoadModel(const char* filename);
	void LoadGlData();
	void LoadModelUniforms(const ShaderProgram& shaderProgram) const;
	void ClearData(bool freeMemory=false) noexcept;
	void ClearGLBuffers() noexcept
	{
		GLuint buffers[] = { verticesBuffer, normalsBuffer, indicesBuffer, texturecoordsBuffer };
		glDeleteVertexArrays(1, &vertexArrayBuffer);
		glDeleteBuffers(4, buffers);
		verticesBuffer = normalsBuffer = indicesBuffer = texturecoordsBuffer = -1;
	};
	void Draw() const;
	glm::mat4 CalculateTransformMat();
	void MoveToCenter();
	
	/* Data */
	Type type;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texturecoords;
	std::vector<unsigned int> indices;

	// 0 - min, 1 - max
	std::array<glm::vec4, 2> boundingBox;
	glm::vec4 position;
	glm::vec3 slopeAngle;
	float scale;

	/* Materials data */
	Material material;

	/* Matrix */
	glm::mat4 transformMat;

	/* Buffers */
	GLuint vertexArrayBuffer;
	GLuint verticesBuffer;
	GLuint normalsBuffer;
	GLuint indicesBuffer;
	GLuint texturecoordsBuffer;

private:
	void SetBoundingBox();
	void ScaleToBoundingBox();

};