#include "DiffuseTexture.h"

#include "gl_core_4_3.h"

#include "ShaderProgram.h"


int DiffuseTexture::GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const
{
	return glGetUniformLocation(shaderProgram.id, "material.diffuseTexture");
}

