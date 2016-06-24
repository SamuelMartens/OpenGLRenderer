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
	
	explicit MixTexture(const Texture& tex ):
		  Texture(tex)
		, mixWeight(0)
	{
		type = Texture::Type::MixTexture;
	};
	
	explicit MixTexture(float mWeight):
		  mixWeight(mWeight)
		, Texture(Texture::Type::MixTexture)
	{};
	
	/* Getters  */
	float GetMixWeight() const noexcept  { return mixWeight; };
	
	/* Setters */
	void SetMixWeight(float newVal )
	{
		assert(newVal>=0 && newVal<=1);
		mixWeight = newVal;
	};
private:
	float mixWeight;
};