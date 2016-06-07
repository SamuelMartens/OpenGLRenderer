#include "Settings.h"
#include "ShaderProgram.h"

#include "gl_core_4_3.h"

#include <iostream>
#include <cassert>

void Settings::PassToShaderProgram(const ShaderProgram& program) const 
{
	assert(program.isLinked());

	GLint cartoonLevelsNumLoc = glGetUniformLocation(program.id, "settings.cartoonLevelsNumber");
	GLint isCartoonLoc = glGetUniformLocation(program.id, "settings.isCartoon");
	GLint isFogLoc = glGetUniformLocation(program.id, "settings.isFog");
	GLint minFogDistanceLoc = glGetUniformLocation(program.id, "settings.minFogDistance");
	GLint maxFogDistanceLoc = glGetUniformLocation(program.id, "settings.maxFogDistance");

	if (-1 == cartoonLevelsNumLoc || -1 == isFogLoc || -1 == isCartoonLoc || -1 == minFogDistanceLoc || -1 == maxFogDistanceLoc)
		std::cout << "Failed to get settings uniform loc. \n";

	glUniform1i(cartoonLevelsNumLoc, cartoonLevels);
	glUniform1i(isFogLoc, static_cast<GLint>(fog));
	glUniform1i(isCartoonLoc, static_cast<GLint>(cartoon));
	glUniform1f(minFogDistanceLoc, static_cast<GLfloat>(minFogDistance));
	glUniform1f(maxFogDistanceLoc, static_cast<GLfloat>(maxFogDistance));
}