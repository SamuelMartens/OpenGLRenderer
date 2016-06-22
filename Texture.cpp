#include "Texture.h"

#include "gl_core_4_3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>
#include <iostream>

void Texture::Load(std::string&& filePath)
{
	int w, h, comp ;
	unsigned char* image = stbi_load(filePath.c_str(), &w, &h, &comp, 0);
	
	if (nullptr == image)
		std::cout << "Failed to load image for texture";



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
	
	initialized = true;
	stbi_image_free(image);
}