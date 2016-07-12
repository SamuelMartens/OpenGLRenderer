#pragma once

#include <map>
#include <vector>
#include <cassert>
#include <memory>

#include "glm\vec3.hpp"

#include "Texture.h"
#include "MixTexture.h"
#include "ShaderProgram.h"
#include "Settings.h"

class Material
{
public:

	Material():
		  Kd(1)
		, Ks(1)
		, Ka(1)
	{};
	
	/* Setters adders */
	void SetTexture(std::shared_ptr<Texture> text);
	/*void AddMixTexture(const MixTexture& text)
	{
		assert(Texture::Type::MixTexture == text.GetType());
		assert(mixTextures.size() <= Settings::Instance().GetMaxMixTextures());
		mixTextures.push_back(text);
	};*/
	/*void AddMixTexture(const MixTexture&& text)
	{
		assert(Texture::Type::MixTexture == text-GetType());
		assert(mixTextures.size() <= Settings::Instance().GetMaxMixTextures());
		mixTextures.push_back(text);
	};*/

	/* Getters */
	Texture* GetTextureWithType(Texture::Type t);
	const std::vector<MixTexture>& GetMixTextureVec() const noexcept { return mixTextures; };

	void PassToShader(const ShaderProgram& shaderProgram) const;
	void BindTextures();
	bool HasTextureWithType(Texture::Type t) const noexcept { return textures.find(t) != textures.end(); };
	void ClearMixTextureVec() noexcept
	{
		mixTextures.clear();
		mixTextures.shrink_to_fit();
	};

	/* Light data */
	glm::vec3 Kd;
	glm::vec3 Ks;
	glm::vec3 Ka;
	
private:
	void LoadMixTextureUniforms(const ShaderProgram& shaderProgram) const;
	void BindStandartTextures();
	void BindMixTextures();

	/* Mix Textures */
	std::vector<MixTexture> mixTextures;

	/* Standart textures */
	std::map<Texture::Type, std::shared_ptr<Texture>> textures;
};