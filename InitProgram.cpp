#include "InitProgram.h"

#include "gl_core_4_3.h"
#include <iostream>

namespace 
{
	void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,const void *param)
	{
		printf("%s:%s[%s] (%d): %s\n",
			glGetString(source),
			glGetString(type),
			glGetString(severity),
			id,
			message);
	}
}

int InitProgram::InitProgram()
{

	int loaded = ogl_LoadFunctions();
	if (loaded == ogl_LOAD_FAILED)
	{
		std::cout << "Failed to load functions. \n";
		int num_failed = loaded - ogl_LOAD_SUCCEEDED;
		printf("Failed to load functions: %i \n", num_failed);
		return -1;
	}

	return 0;
}


void InitProgram::SetDebugCallback()
{
	glDebugMessageCallback(debugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}