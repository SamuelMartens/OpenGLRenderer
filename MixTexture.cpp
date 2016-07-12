#include "MixTexture.h"

#include <cassert>
#include <cstring>
#include <iostream>

#include "gl_core_4_3.h"

#include "ShaderProgram.h"

int MixTexture::GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const
{
	assert(0 <= indexNumber);
	char mixTextureUniformName[100];
	sprintf_s(mixTextureUniformName, "material.mixTextures[%d].texture", indexNumber);
	int loc = glGetUniformLocation(shaderProgram.id, mixTextureUniformName);

	if (-1 == loc)
		std::cout << "Failed to get mix texture uniform location. \n";
	return loc;
};

void MixTexture::LoadUniforms(const ShaderProgram& shaderProgram) const
{
	assert(0 <= indexNumber);
	int loc = GetMixWeightLocation(shaderProgram);
	glUniform1f(loc, mixWeight);
}

int MixTexture::GetMixWeightLocation(const ShaderProgram& shaderProgram) const
{
	assert(0 <= indexNumber);
	char mixTextureUniformName[100];
	sprintf_s(mixTextureUniformName, "material.mixTextures[%d].mixWeight", indexNumber);
	int loc = glGetUniformLocation(shaderProgram.id, mixTextureUniformName);

	if (-1 == loc)
		std::cout << "Failed to get mix texture uniform location. \n";
	return loc;
}

int MixTexture::ActivateGLTextureSlot() const
{
	assert(0 <= indexNumber);
	int slotNumber = static_cast<int>(type) + indexNumber;
	glActiveTexture(GL_TEXTURE0 + indexNumber);
	return slotNumber;
}