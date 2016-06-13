#pragma once

#include "ShaderProgram.h"

#include "gl_core_4_3.h"

#include <string>
#include <cassert>

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
	{
		glGenTextures(1, &id);
		if (GL_INVALID_VALUE == id)
			std::cout << "Failed to generate texture";
	};
	Texture(Texture::Type t):
		  initialized(false)
		, type(t)
	{
		glGenTextures(1, &id);
		if (GL_INVALID_VALUE == id)
			std::cout << "Failed to generate texture";
	};
	virtual ~Texture(){ glDeleteTextures(1, &id); };

	/* Getters */
	GLuint GetId() const noexcept { return id; };
	bool isInitialized() const noexcept { return initialized; };

	void Bind() const 
	{
		assert(initialized);
		glBindTexture(GL_TEXTURE_2D, id);
	};
	void Load(std::string&& filePath);
	void PassToShader(const ShaderProgram) const;
	
	Type type;

private:
	GLuint id;
	bool initialized;

};

