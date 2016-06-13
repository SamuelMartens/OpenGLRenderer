#include "Material.h"
#include "Texture.h"

#include <cassert>
#include <map>

void Material::SetTexture(const Texture& text)
{
	assert(text.type != Texture::Type::MixTexture);
	auto res = texture.insert({text.type, text});
	
	if (res.second)
		return;
	
	res.first->second = text;
}



void Material::SetTexture(const Texture&& text)
{
	assert(text.type != Texture::Type::MixTexture);
	auto res = texture.insert({text.type, text});
	
	if (res.second)
		return;
	
	res.first->second = text;
}

Texture* Material::GetTextureWithType(Texture::Type t)
{
	auto res = textures.find(t);
	if (textures.end() == res)
		return nullptr;
	
	return &(res->text);
}