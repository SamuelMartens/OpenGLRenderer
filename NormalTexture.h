#pragma once

#include <vector>

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
	{};
	explicit NormalTexture(std::vector<float>& tangentsVal) :
		  Texture(Texture::Type::Normal)
		, tangents(tangentsVal)
		, tangentsBuffer(0)
	{};
	explicit NormalTexture(std::vector<float>&& tangensVal) :
		Texture(Texture::Type::Normal)
		, tangents(tangensVal)
	{};

	NormalTexture(const NormalTexture&) = default;

	~NormalTexture() = default;

	/* Getters */
	const std::vector<float>& GetTangens() const noexcept { return tangents; };
	GLuint GetTangentsBuffer() const noexcept { return tangentsBuffer; };

	/* Setters */
	void SetTangens(const std::vector<float>& tangentsVal) noexcept { tangents = tangentsVal; };

	void CalculateTangens(const Model& model);
	void CleanGLBuffers() noexcept override
	{
		if (tangentsBuffer)
		{
			glDeleteBuffers(1, &tangentsBuffer);
			tangentsBuffer = 0;
		}
	}
	void LoadTangensInGLModelBuffer(const Model& model);
	virtual int GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const override;
	virtual void LoadGLBuffers(const Model& model) override
	{
		CalculateTangens(model);
		LoadTangensInGLModelBuffer(model);
	};


private:
	std::vector<float> tangents;
	GLuint tangentsBuffer;
};

