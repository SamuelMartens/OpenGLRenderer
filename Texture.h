#pragma once


#include <string>
#include <cassert>

#include "gl_core_4_3.h"

#include "ShaderProgram.h"

class Texture
{
public:
	enum class Type
	{
		Diffuse = 0,
		Normal = 1,
		Transparent = 2,
		MixTexture = 3
	};

	Texture() :
		  type(Type::Diffuse)
		, initialized(false)
		, id(-1)
	{};

	explicit Texture(Texture::Type t):
		  initialized(false)
		, type(t)
		, id(-1)
	{};

	virtual ~Texture(){  };

	/* Getters */
	GLuint GetId() const noexcept { return id; };
	bool isInitialized() const noexcept { return initialized; };

	void Bind() const 
	{
		assert(initialized);
		glBindTexture(GL_TEXTURE_2D, id);
	};
	int ActivateGLTextureSlot(int mixTextureNum=-1) const;
	int GetUniformLocation(const ShaderProgram& shaderProgram, int mixTextureNum=-1) const; 
	void Load(std::string&& filePath, int mixTextureNum=-1);
	//void PassToShader(const ShaderProgram& ) const;
	void ClearGLData() noexcept
	{
		glDeleteTextures(1, &id);
		id = -1;
	}
	
	Type type;

private:
	GLuint id;
	bool initialized;
};

