#pragma once

#include "Texture.h"

#include <cassert>

class MixTexture: public Texture
{
public:
	MixTexture():
		  mixWeight(0)
		, Texture(Texture::Type::MixTexture)
	{};
	
	MixTexture(const Texture& tex ):
		  Texture(tex)
		, minWeight(0)
	{ type = Texture::Type::MixTexture };
	
	MixTexture(float mWeight):
		  minWeight(mWeight)
		, Texture(Texture::Type::MixTexture)
	{};
	
	/* Getters  */
	void GetMixWeight() noexcept const { return mixWeight; };
	
	/* Setters */
	void SetMixWeight(float newVal )
	{
		assert(newVal>=0 && newVal<=1);
		mixWeight = newVal;
	};
private:
	float mixWeight;
}