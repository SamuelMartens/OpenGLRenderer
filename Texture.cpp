#include "Texture.h"

#include <string>
#include <iostream>

#include "gl_core_4_3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ShaderProgram.h"

void Texture::Load(std::string&& filePath, const ShaderProgram& shaderProgram)
{
	int w, h, comp ;
	unsigned char* image = stbi_load(filePath.c_str(), &w, &h, &comp, 0);
	
	if (nullptr == image)
		std::cout << "Failed to load image for texture";

	int slotNum = ActivateGLTextureSlot();
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
	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glUniform1i(GetSamplerUniformLocation(shaderProgram), slotNum);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	initialized = true;
	stbi_image_free(image);
}

int Texture::ActivateGLTextureSlot() const
{
	int intType = static_cast<int>(type);
	glActiveTexture(GL_TEXTURE0 + intType);
	return intType;
}

