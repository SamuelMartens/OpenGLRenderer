#pragma once

#include <string>
#include <memory>

#include "DiffuseTexture.h"
#include "TransparentTexture.h"
#include "ShaderProgram.h"

class Resources
{
public:

	Resources() :
		  initialized(false)
		, pathSphereModel("E:\\C++\\OpenGLtutorial\\resources\\sphere.obj")
	{};

	void Init(const ShaderProgram& shaderProgram);

	/* Setters */

	/* Getters */
	bool IsInitialized() const noexcept { return initialized; };
	const std::string& GetPathSphereModel() const noexcept { return pathSphereModel; };

	DiffuseTexture default_texture;
	TransparentTexture default_transparent_texture;
	

private:
	const std::string pathSphereModel;
	bool initialized;
};

