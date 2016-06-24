#include "Texture.h"

#include <cstring>
#include <string>
#include <iostream>

#include "gl_core_4_3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ShaderProgram.h"

void Texture::Load(std::string&& filePath, const ShaderProgram& shaderProgram, int mixTextureNum)
{
	int w, h, comp ;
	unsigned char* image = stbi_load(filePath.c_str(), &w, &h, &comp, 0);
	
	if (nullptr == image)
		std::cout << "Failed to load image for texture";

	int slotNum = ActivateGLTextureSlot(mixTextureNum);
	glGenTextures(1, &id);
	if (GL_INVALID_VALUE == id)
		std::cout << "Failed to generate texture";

	glBindTexture(GL_TEXTURE_2D, id);
	switch (comp)
	{
	case 4:
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, w, h);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image);
		break;
	case 3:
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, w, h);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, image);
		break;
	default:
		std::cout << "Undefined texture comp variable \n";
		break;
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glUniform1i(GetUniformLocation(shaderProgram, mixTextureNum), slotNum);

	initialized = true;
	stbi_image_free(image);
}

int Texture::ActivateGLTextureSlot(int mixTextureNum) const
{
	switch (type)
	{
	case Texture::Type::Diffuse:
		glActiveTexture(GL_TEXTURE0);
		return static_cast<int>(type);
		break;
	case Texture::Type::Normal:
		glActiveTexture(GL_TEXTURE1);
		return static_cast<int>(type);
		break;
	case Texture::Type::Transparent:
		glActiveTexture(GL_TEXTURE2);
		return static_cast<int>(type);
		break;
	case Texture::Type::MixTexture:
		assert(mixTextureNum >= 0);
		glActiveTexture(GL_TEXTURE3 + mixTextureNum);
		return static_cast<int>(type) + mixTextureNum;
		break;
	default:
		return -1;
		break;
	}
}

int Texture::GetUniformLocation(const ShaderProgram& shaderProgram, int mixTextureNum) const
{
	int loc;
	char mixTextureUniformName[100];

	switch (type)
	{
	case Texture::Type::Diffuse:
		loc = glGetUniformLocation(shaderProgram.id, "diffuseTexture");
		break;
	case Texture::Type::Normal:
		loc = glGetUniformLocation(shaderProgram.id, "normalTexture");
		break;
	case Texture::Type::Transparent:
		loc = glGetUniformLocation(shaderProgram.id, "transparentTexture");
		break;
	case Texture::Type::MixTexture:
		assert(mixTextureNum > 0);
		sprintf_s(mixTextureUniformName, "mixTextures[%d]", mixTextureNum);
		loc = glGetUniformLocation(shaderProgram.id, mixTextureUniformName);
		break;
	default:
		break;
	}

	if (-1 == loc)
		std::cout << "Failed to get texture uniform location. \n";
	return loc;
};