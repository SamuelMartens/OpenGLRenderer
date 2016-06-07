#pragma once

#include "ShaderProgram.h"

#include <iostream>

#include "gl_core_4_3.h"

struct Settings
{
	Settings() :
		fog(false),
		cartoon(false),
		cartoonLevels(4),
		maxLightNumber(5),
		minFogDistance(0),
		maxFogDistance(0)
		{};
	~Settings() = default;

	/* Fog settings */
	bool fog;
	float minFogDistance;
	float maxFogDistance;

	/* Toon shading settings */
	bool cartoon;
	unsigned cartoonLevels;
	bool flatShading;
	
	/* Getters */
	unsigned int GetMaxLightNumber() const noexcept { return maxLightNumber; };

	/* Other functions */
	void PassToShaderProgram(const ShaderProgram& program) const;
	bool HasEffects() const noexcept { return (fog || cartoon); };

private:
	const unsigned maxLightNumber;
};