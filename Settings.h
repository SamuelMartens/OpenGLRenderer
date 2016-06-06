#pragma once

#include <iostream>

#include "gl_core_4_3.h"

struct Settings
{
	Settings() :
		fog(false),
		cartoon(false),
		cartoonLevels(4),
		maxLightNumber(5)
		{};
	~Settings() = default;

	bool fog;
	bool cartoon;
	unsigned cartoonLevels;
	bool flatShading;
	
	/* Getters */
	unsigned int GetMaxLightNumber() const noexcept { return maxLightNumber; };

	/* Other functions */
	void PassToShaderProgram(const GLuint programId) const;
	bool HasEffects() const noexcept { return (fog || cartoon); };

private:
	unsigned maxLightNumber;
};