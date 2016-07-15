#include "DiffuseTexture.h"

#include <iostream>

#include "gl_core_4_3.h"

#include "ShaderProgram.h"


int DiffuseTexture::GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const
{
	int loc = glGetUniformLocation(shaderProgram.id, "material.diffuseTexture");
	
	if (-1 == loc)
		std::cout << "Failed to get diffuse texture uniform location \n";

	return loc;
}

