#pragma once

#include <cassert>
#include <vector>
#include <memory>

#include "Texture.h"
#include "Settings.h"

class MixTexture: public Texture
{
public:
	MixTexture():
		  mixWeight(0)
		, Texture(Texture::Type::MixTexture)
		, indexNumber(-1)
	{};
	explicit MixTexture(float newMixWeight) :
		  mixWeight(newMixWeight)
		, Texture(Texture::Type::MixTexture)
		, indexNumber(-1)
	{};

	/* Getters  */
	float GetMixWeight() const noexcept { return mixWeight; };
	int GetIndexNumber() const noexcept { return indexNumber; };
	int GetMixWeightLocation(const ShaderProgram& shaderProgram) const;
	virtual int GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const override;
	virtual void LoadUniforms(const ShaderProgram& shaderProgram) const override;
	virtual int ActivateGLTextureSlot() const override;
	
	/* Setters */
	void SetMixWeight(float newMixWeight)
	{
		assert(newMixWeight >= 0 && newMixWeight <= 1);
		mixWeight = newMixWeight;
	}
	void SetindexNumber(const std::vector<std::shared_ptr<MixTexture>>& mixTextureContainer)
	{
		// MUST be called before add this texture to passed container
		assert(mixTextureContainer.size() < Settings::Instance().GetMaxMixTextures());
		indexNumber = mixTextureContainer.size();
	}

private:
	float mixWeight;
	int indexNumber;
};