#pragma once

#include "glm\vec3.hpp"

#include "Texture.h"
#include "MixTexture.h"

#include <map>
#include <vector>

class Material
{
public:
	Material():
		  Kd(1)
		, Ks(1)
		, Ka(1)
	{};
	
	bool HasTextureWithType(Texture::Type t) const noexcept { return textures.find(t) != textures.end(); };
	void SetTexture(const Texture& text);
	void SetTexture(const Texture&& text);
	
	/* Getters */ 
	Texture* GetTextureWithType(Texture::Type t); 
	
	/* Light data */
	glm::vec3 Kd;
	glm::vec3 Ks;
	glm::vec3 Ka;
	
	/* Mix Textures */
	std::vector<MixTexture> mixTextures;
	
private:
	std::map<Texture::Type, Texture> textures;
}