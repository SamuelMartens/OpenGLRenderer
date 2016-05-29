#include "Settings.h"

#include "gl_core_4_3.h"

#include <iostream>
#include <cassert>

void Settings::PassToShaderProgram(GLuint programId) const 
{
	GLint cartoonLevelsNumLoc = glGetUniformLocation(programId, "settings.cartoonLevelsNumber");
	GLint isFogLoc = glGetUniformLocation(programId, "settings.isFog");
	GLint isCartoonLoc = glGetUniformLocation(programId, "settings.isCartoon");

	if (-1 == cartoonLevelsNumLoc || -1 == isFogLoc)
		std::cout << "Failed to get settings uniform loc. \n";

	glUniform1i(cartoonLevelsNumLoc, cartoonLevels);
	glUniform1i(isFogLoc, static_cast<GLint>(fog));
	glUniform1i(isCartoonLoc, static_cast<GLint>(cartoon));

	assert(initialized);
	if (HasEffects())
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &useEffectsSubroutione);
	else
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &noEffectsSubroutine);
		
}