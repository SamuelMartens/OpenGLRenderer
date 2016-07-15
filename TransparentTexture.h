#pragma once

#include <cassert>

#include "Texture.h"
#include "ShaderProgram.h"

class TransparentTexture : public Texture
{
public:
	TransparentTexture() :
		  transparentEdge(1)
		, Texture(Texture::Type::Transparent)
	{};

	explicit TransparentTexture(float t) :
		  transparentEdge(t)
		, Texture(Texture::Type::Transparent)
	{};
	~TransparentTexture() = default;

	/* Getters */
	float GetTransparentEdge() const noexcept { return transparentEdge; };
	int GetTransparentEdgeLocation(const ShaderProgram& shaderProgram) const;
	virtual int GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const override;

	/* Setters */
	void SetTransparentEdge(float newVal)
	{
		assert(newVal <= 1 && newVal >= 0);
		transparentEdge = newVal;
	};


	virtual void LoadUniforms(const ShaderProgram& shaderProgram) const override;

private:
	float transparentEdge;
};

