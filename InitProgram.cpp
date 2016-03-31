#include "InitProgram.h"

#include "gl_core_4_3.h"
#include <iostream>

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