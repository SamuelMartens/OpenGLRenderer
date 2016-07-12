#pragma once

#include "DiffuseTexture.h"
#include "ShaderProgram.h"

class Resources
{
public:
	Resources() :
		initialized(false)
	{};

	void Init(const ShaderProgram& shaderProgram);

	/* Getters */
	bool isInitialized() const noexcept { return initialized; };

	DiffuseTexture default_texture;
	//Texture light_sources_texture;

private:
	bool initialized;
};

