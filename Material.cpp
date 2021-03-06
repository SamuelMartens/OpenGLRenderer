#include "Material.h"

#include <cassert>
#include <map>
#include <iostream>
#include <memory>

#include "gl_core_4_3.h"

#include "Texture.h"
#include "Settings.h"

void Material::SetTexture(std::shared_ptr<Texture> text)
{
	assert(text->GetType() != Texture::Type::MixTexture);
	auto res = textures.insert({ text->GetType(), text });

	if (res.second)
		return;

	res.first->second = std::move(text);
}

Texture* Material::GetTextureWithType(Texture::Type t)
{
	auto res = textures.find(t);
	if (textures.end() == res)
		return nullptr;
	
	return res->second.get();
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

	uniformLoc = glGetUniformLocation(shaderProgram.id, "material.mixTexturesNumber");
	if (-1 == uniformLoc)
		std::cout << "Failed to get material uniform \n";
	else
		glUniform1i(uniformLoc, mixTextures.size());

	uniformLoc = glGetUniformLocation(shaderProgram.id, "materialVertex.hasNormalTexture");
	if (-1 == uniformLoc)
		std::cout << "Failed to get material uniform \n";
	else
		glUniform1i(uniformLoc, static_cast<int>(HasTextureWithType(Texture::Type::Normal)));


	LoadStandartTexturesUniforms(shaderProgram);
	LoadMixTextureUniforms(shaderProgram);
}

void Material::BindTextures()
{
	BindStandartTextures();
	BindMixTextures();
}

void Material::BindStandartTextures()
{
	const Texture* texture = GetTextureWithType(Texture::Type::Diffuse);
	if (texture)
		texture->Bind();
	else
		Settings::Instance().resources.default_texture.Bind();

	texture = GetTextureWithType(Texture::Type::Transparent);
	if (texture)
		texture->Bind();
	else
		Settings::Instance().resources.default_transparent_texture.Bind();

	texture = GetTextureWithType(Texture::Type::Normal);
	if (texture)
		texture->Bind();

}

void Material::BindMixTextures()
{
	for (unsigned i = 0; i < mixTextures.size(); ++i)
	{
		mixTextures[i]->Bind();
	}
}

void Material::LoadMixTextureUniforms(const ShaderProgram& shaderProgram) const
{
	for (unsigned i = 0; i < mixTextures.size(); ++i)
	{
		int weightLoc = mixTextures[i]->GetMixWeightLocation(shaderProgram);
		mixTextures[i]->LoadUniforms(shaderProgram);
	}
}

void Material::LoadStandartTexturesUniforms(const ShaderProgram& shaderProgram) const
{
	for (auto it = textures.begin(); it != textures.end(); ++it)
	{
		it->second->LoadUniforms(shaderProgram);
	}
}

void Material::LoadTexuresGLBuffers(const Model& model)
{
	// Here we works only with standart textures
	for (auto texture : textures)
	{
		texture.second->LoadGLBuffers(model);
	}
}