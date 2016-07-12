#pragma once

#include "ShaderProgram.h"

#include <iostream>

#include "gl_core_4_3.h"

#include "Resources.h"

struct Settings
{
	/* Fog settings */
	bool fog;
	float minFogDistance;
	float maxFogDistance;

	/* Toon shading settings */
	bool cartoon;
	unsigned cartoonLevels;
	bool flatShading;
	
	/* Getters */
	static Settings& Instance()
	{
		static Settings instance;
		return instance;
	}
	unsigned int GetMaxLightNumber() const noexcept { return maxLightNumber; };
	unsigned int GetMaxMixTextures() const noexcept { return maxMixTextures; };

	/* Other functions */
	void PassToShaderProgram(const ShaderProgram& program) const;
	bool HasEffects() const noexcept { return (fog || cartoon); };

	/* Resources */
	Resources resources;

	Settings(const Settings&) = delete;
	void operator=(const Settings&) = delete;
	~Settings() = default;

private:
	Settings() :
		fog(false),
		cartoon(false),
		cartoonLevels(4),
		maxLightNumber(5),
		maxMixTextures(5),
		minFogDistance(0),
		maxFogDistance(0)
	{};

	// Don't forget to change this value in shader
	const unsigned maxLightNumber;
	const unsigned maxMixTextures;
};