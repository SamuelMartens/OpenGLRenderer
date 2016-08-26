#pragma once

#include <vector>
#include <string>

#include "Texture.h"
#include "ShaderProgram.h"

class CubeTexture : public Texture
{
public:
	CubeTexture() :
		Texture(Texture::Type::Cube)
	{};
	~CubeTexture() = default;

	virtual void Bind() const override
	{
		assert(initialized);
		ActivateGLTextureSlot();
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	};

	virtual int GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const override {};
	virtual void LoadFewTextures(std::vector<std::string>&& filesPathes, const ShaderProgram& shaderProgram) override;
};

