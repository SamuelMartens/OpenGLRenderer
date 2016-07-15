#include "TransparentTexture.h"

#include <iostream>

#include "gl_core_4_3.h"

#include "ShaderProgram.h"

int TransparentTexture::GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const
{
	int loc = glGetUniformLocation(shaderProgram.id, "material.transparentTexture.texture");

	if (-1 == loc)
		std::cout << "Failed to get transparent texture uniform location \n";

	return loc;
}

int TransparentTexture::GetTransparentEdgeLocation(const ShaderProgram& shaderProgram) const
{
	int loc = glGetUniformLocation(shaderProgram.id, "material.transparentTexture.transparentEdge");

	if (-1 == loc)
		std::cout << "Failed to get transparent texture uniform location \n";

	return loc;
}


void TransparentTexture::LoadUniforms(const ShaderProgram& shaderProgram) const
{
	glUniform1f(GetTransparentEdgeLocation(shaderProgram), transparentEdge);
}