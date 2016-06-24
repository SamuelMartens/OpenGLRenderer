#include "Material.h"

#include <cassert>
#include <map>
#include <iostream>

#include "gl_core_4_3.h"

#include "Texture.h"

void Material::SetTexture(const Texture& text)
{
	assert(text.type != Texture::Type::MixTexture);
	auto res = textures.insert({text.type, text});
	
	if (res.second)
		return;
	
	res.first->second = text;
}



void Material::SetTexture(const Texture&& text)
{
	assert(text.type != Texture::Type::MixTexture);
	auto res = textures.insert({text.type, text});
	
	if (res.second)
		return;
	
	res.first->second = text;
}

Texture* Material::GetTextureWithType(Texture::Type t)
{
	auto res = textures.find(t);
	if (textures.end() == res)
		return nullptr;
	
	return &(res->second);
}

void Material::PassToShader(const ShaderProgram& shaderProgram) const
{
	GLuint uniformLoc = glGetUniformLocation(shaderProgram.id, "material.Kd");
	if (-1 == uniformLoc)
		std::cout << "Failed to get material uniform \n";
	else
		glUniform3fv(uniformLoc, 1, &Kd[0]);

	uniformLoc = glGetUniformLocation(shaderProgram.id, "material.Ks");
	if (-1 == uniformLoc)
		std::cout << "Failed to get material uniform \n";
	else
		glUniform3fv(uniformLoc, 1, &Ks[0]);

	uniformLoc = glGetUniformLocation(shaderProgram.id, "material.Ka");
	if (-1 == uniformLoc)
		std::cout << "Failed to get material uniform \n";
	else
		glUniform3fv(uniformLoc, 1, &Ka[0]);
}