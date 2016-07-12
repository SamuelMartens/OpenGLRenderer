#include "Resources.h"

#include "Texture.h"
#include "ShaderProgram.h"

void Resources::Init(const ShaderProgram& shaderProgram)
{
	default_texture.Load("E:\\C++\\OpenGLtutorial\\resources\\default_texture.jpg", shaderProgram);
	//light_sources_texture.Load("E:\\C++\\OpenGLtutorial\\resources\\light_texture.png", shaderProgram);

	initialized = true;
}
