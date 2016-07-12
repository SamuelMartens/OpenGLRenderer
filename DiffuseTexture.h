#pragma once

#include "Texture.h"
#include "ShaderProgram.h"

class DiffuseTexture: public Texture 
{
public:
	DiffuseTexture() :
		Texture(Texture::Type::Diffuse)
	{};

	virtual int GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const override;
};

