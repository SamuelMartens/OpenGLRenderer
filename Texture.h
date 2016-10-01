#pragma once

#include <string>
#include <vector>
#include <cassert>

#include "gl_core_4_3.h"

#include "ShaderProgram.h"

// For preventing cycling dependency
class Model;

class Texture
{
public:

	enum class Type
	{
		Diffuse = 0,
		Normal = 1,
		Transparent = 2,
		Cube = 3,
		MixTexture = 4
	};

	explicit Texture(Texture::Type t) :
		initialized(false)
		, type(t)
		, id(-1)
	{};

	virtual ~Texture() {};

	/* Getters */
	GLuint GetId() const noexcept { return id; };
	bool isInitialized() const noexcept { return initialized; };
	Texture::Type GetType() { return type; };

	virtual void Bind() const 
	{
		assert(initialized);
		ActivateGLTextureSlot();
		glBindTexture(GL_TEXTURE_2D, id);
		
	};
	
 	virtual int GetSamplerUniformLocation(const ShaderProgram& shaderProgram) const = 0; 
	virtual int ActivateGLTextureSlot() const;
	virtual void Load(std::string&& filePath, const ShaderProgram& shaderProgram);
	// Empty virtual methods
	virtual void LoadFewTextures(std::vector<std::string>&& filesPathes, const ShaderProgram& shaderProgram) {};
	virtual void LoadUniforms(const ShaderProgram& shaderProgram) const {};
	virtual void LoadGLBuffers(const Model& model) {};
	virtual void CleanGLBuffers() noexcept {};

	void ClearGLData() noexcept
	{
		glDeleteTextures(1, &id);
		id = -1;
	}

protected:

	Type type;
	GLuint id;
	bool initialized;
};

