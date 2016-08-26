#pragma once

#include <string>

#include "DiffuseTexture.h"
#include "TransparentTexture.h"
#include "ShaderProgram.h"

class Resources
{
public:
	Resources() :
		  initialized(false)
		, pathToSkyBoxModel("E:\\C++\\OpenGLtutorial\\resources\\cube.obj")
	{};

	void Init(const ShaderProgram& shaderProgram);

	/* Setters */
	void SetSkyBoxModelPath(std::string&& newPath) { pathToSkyBoxModel = newPath; };

	/* Getters */
	bool IsInitialized() const noexcept { return initialized; };
	const std::string& GetPathToSkyBoxModel() const noexcept { return pathToSkyBoxModel; };

	DiffuseTexture default_texture;
	TransparentTexture default_transparent_texture;
	

private:
	
	std::string pathToSkyBoxModel;
	bool initialized;
};

