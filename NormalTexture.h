#pragma once

#include <vector>
#include <string>

#include "glm\vec3.hpp"
#include "gl_core_4_3.h"

#include "Texture.h"
#include "model.h"
#include "ShaderProgram.h"

class NormalTexture : public Texture
{
public:
	NormalTexture() :
		  Texture(Texture::Type::Normal)
		, tangentsBuffer(0)
		, bitangentsBuffer(0)
	{};

	NormalTexture(const NormalTexture&) = default;

	~NormalTexture() = default;

	/* Getters */
	const std::vector<float>& GetTangens() const noexcept { return tangents; };
	GLuint GetTangentsBuffer() const noexcept { return tangentsBuffer; };
	GLuint GetBitangentsBuffer() const noexcept { return bitangentsBuffer; };
	
	/* Setters */

	void CalculateTangAndBitang(const Model& model);
	void CleanGLBuffers() noexcept override
	{
		if (tangentsBuffer)
		{
			glDeleteBuffers(1, &tangentsBuffer);
			tangentsBuffer = 0;
		}
		if (bitangentsBuffer)
		{
			glDeleteBuffers(1, &bitangentsBuffer);
			bitangentsBuffer = 0;
		}
	}
	void LoadGLModelBuffer(const Model& model);
	virtual int GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const override;
	virtual void LoadGLBuffers(const Model& model) override
	{
		CalculateTangAndBitang(model);
		LoadGLModelBuffer(model);
	};


private:
	std::vector<float> tangents;
	std::vector<float> bitangets;

	GLuint tangentsBuffer;
	GLuint bitangentsBuffer;
};

