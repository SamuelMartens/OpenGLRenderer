#pragma once

#include "gl_core_4_3.h"

struct Settings
{
	Settings() :
		fog(false),
		cartoon(false),
		cartoonLevels(4),
		maxLightNumber(5),
		noEffectsSubroutine(-1),
		useEffectsSubroutione(-1),
		initialized(false)
	{
	};
	~Settings() = default;

	bool fog;
	bool cartoon;
	unsigned cartoonLevels;
	bool flatShading;
	
	/* Getters */
	unsigned int GetMaxLightNumber() const noexcept { return maxLightNumber; };
	bool IsInitialized() const noexcept { return initialized; };

	/* Other functions */
	void PassToShaderProgram(GLuint programId) const;
	bool HasEffects() const noexcept { return (fog || cartoon); };
	void InitSuboroutines(GLuint programId) 
	{
		noEffectsSubroutine = glGetSubroutineIndex(programId, GL_FRAGMENT_SHADER, "NoEffects");
		useEffectsSubroutione = glGetSubroutineIndex(programId, GL_FRAGMENT_SHADER, "UseEffects");
		initialized = true;
	};

private:
	unsigned maxLightNumber;
	bool initialized;

	GLuint noEffectsSubroutine;
	GLuint useEffectsSubroutione;

};