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
	if (4 == comp)
	{
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, w, h);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	

	initialized = true;
	stbi_image_free(image);
}