#pragma once

#include <array>
#include <vector>

#include "glm\mat4x4.hpp"
#include "glm\vec4.hpp"
#include "glm\vec3.hpp"
#include "gl_core_4_3.h"

#include "Material.h"
#include "Resources.h"

class Model
{
public:

	enum class Type
	{
		commonModel,
		lightModel,
		skyBoxModel
	};

	Model();
	Model(const Model& m) = default;
	Model& operator=(const Model&) = default;
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
		Texture* normalTexture = material.GetTextureWithType(Texture::Type::Normal);
		if (normalTexture)
			normalTexture->CleanGLBuffers();
		verticesBuffer = normalsBuffer = indicesBuffer = texturecoordsBuffer = vertexArrayBuffer = 0;
	};
	void Draw(const Resources& resources);
	glm::mat4 CalculateTransformMat();
	void MoveToCenter();
	

	/* Getters */
	const glm::mat4& GetModelMat() const noexcept { return transformMat; };
	glm::mat4& GetModelMat()
	{
		// Here to avoid code duplication for const and non const getter.
		// This code is madness but I trust Scott Meyer, as it is his advice :)
		return const_cast<glm::mat4&>(
			static_cast<const Model&>(*this).GetModelMat());
	};
	Type GetType() const noexcept { return type; };
	const std::vector<float>& GetVertices() const noexcept { return vertices; };
	std::vector<float>& GetVertices()
	{
		return const_cast<std::vector<float>&>(
			static_cast<const Model&>(*this).GetVertices());
	}
	const std::vector<float>& GetNormals() const noexcept { return normals; };
	std::vector<float>& GetNormals()
	{
		return const_cast<std::vector<float>&>(
			static_cast<const Model&>(*this).GetNormals());
	}
	const std::vector<float>& GetTexCoords() const noexcept { return texturecoords; };
	std::vector<float>& GetTexCoords()
	{
		return const_cast<std::vector<float>&>(
			static_cast<const Model&>(*this).GetTexCoords());
	}
	const std::vector<unsigned>& GetIndices() const noexcept { return indices; };
	std::vector<unsigned>& GetIndices()
	{
		return const_cast<std::vector<unsigned>&>(
			static_cast<const Model&>(*this).GetIndices());
	}

	/* Setters */
	void SetType(Type newType) { type = newType; };

	// 0 - min, 1 - max
	std::array<glm::vec4, 2> boundingBox;
	glm::vec4 position;
	glm::vec3 slopeAngle;
	float scale;

	/* Materials data */
	Material material;

	/* Buffers */
	GLuint vertexArrayBuffer;
	GLuint verticesBuffer;
	GLuint normalsBuffer;
	GLuint indicesBuffer;
	GLuint texturecoordsBuffer;

private:

	/* Data */
	Type type;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texturecoords;
	std::vector<unsigned> indices;

	/* Matrix */
	glm::mat4 transformMat;

	void SetBoundingBox();
	void ScaleToBoundingBox();

};