#include "Settings.h"

#include "gl_core_4_3.h"

#include <iostream>
#include <cassert>

void Settings::PassToShaderProgram(GLuint programId) const 
{
	GLint cartoonLevelsNumLoc = glGetUniformLocation(programId, "settings.cartoonLevelsNumber");
	GLint isCartoonLoc = glGetUniformLocation(programId, "settings.isCartoon");
	GLint isFogLoc = glGetUniformLocation(programId, "settings.isFog");

	if (-1 == cartoonLevelsNumLoc || -1 == isFogLoc || -1 == isCartoonLoc)
		std::cout << "Failed to get settings uniform loc. \n";

	glUniform1i(cartoonLevelsNumLoc, cartoonLevels);
	glUniform1i(isFogLoc, static_cast<GLint>(fog));
	glUniform1i(isCartoonLoc, static_cast<GLint>(cartoon));
}