#pragma once

#include "Texture.h"
#include "ShaderProgram.h"

class DiffuseTexture: public Texture 
{
public:
	DiffuseTexture() :
		Texture(Texture::Type::Diffuse)
	{};
	~DiffuseTexture() = default;

	virtual int GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const override;
};

