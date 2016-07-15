#pragma once

#include "DiffuseTexture.h"
#include "TransparentTexture.h"
#include "ShaderProgram.h"

class Resources
{
public:
	Resources() :
		initialized(false)
	{};

	void Init(const ShaderProgram& shaderProgram);

	/* Getters */
	bool IsInitialized() const noexcept { return initialized; };

	DiffuseTexture default_texture;
	TransparentTexture default_transparent_texture;

private:
	bool initialized;
};

