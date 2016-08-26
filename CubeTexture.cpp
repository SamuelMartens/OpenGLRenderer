#include "CubeTexture.h"

#include <vector>
#include <iostream>
#include <string>
#include <cassert>

#include "gl_core_4_3.h"
#include "stb_image.h"

#include "ShaderProgram.h"

void CubeTexture::LoadFewTextures(std::vector<std::string>&& filesPathes, const ShaderProgram& shaderProgram)
{
	constexpr unsigned targetsNum = 6;
	assert(targetsNum == filesPathes.size());

	const int slotNum = ActivateGLTextureSlot();

	glGenTextures(1, &id);
	if (GL_INVALID_VALUE == id)
	{
		std::cout << "Failed to generate cube texture. \n";
		return;
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	GLuint targets[targetsNum] = 
	{
		  GL_TEXTURE_CUBE_MAP_POSITIVE_X
		, GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		, GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
		, GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	GLint w, h, comp;

	// Parse first image to get pictures width and height
	unsigned char* image = stbi_load(filesPathes[0].c_str(), &w, &h, &comp, 0);
	if (nullptr == image)
	{
		std::cout << "Failed to load image for cube texture. \n";
		return;
	}

	switch (comp)
	{
	case 4:
		glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, w, h);
		glTexSubImage2D(targets[0], 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image);
		break;
	case 3:
		glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGB8, w, h);
		glTexSubImage2D(targets[0], 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, image);
		break;
	default:
		std::cout << "Undefined texture comp variable. \n";
		return;
		break;
	}

	stbi_image_free(image);

	for (unsigned i = 1; i < targetsNum; ++i)
	{
		image = stbi_load(filesPathes[i].c_str(), &w, &h, &comp, 0);
		if (nullptr == image)
		{
			std::cout << "Failed to load image for cube texture. \n";
			return;
		}
		switch (comp)
		{
		case 4:
			glTexSubImage2D(targets[i], 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image);
			break;
		case 3:
			glTexSubImage2D(targets[i], 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, image);
			break;
		default:
			std::cout << "Undefined texture comp variable. \n";
			return;
			break;
		}

		stbi_image_free(image);
	}

	initialized = true;

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}