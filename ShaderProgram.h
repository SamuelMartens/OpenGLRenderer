#pragma once

#include <iostream>

#include "Shader.h"

#include "gl_core_4_3.h"

class ShaderProgram
{
public:
	ShaderProgram():
		linked(false)
	{
		id  = glCreateProgram();
		if (0 == id)
			std::cout<<"Failed to create shader program \n";
	};
	ShaderProgram(Shader& vs, Shader& fs );
	ShaderProgram(Shader&& vs, Shader&& fs);
	
	~ShaderProgram()
	{
		glDeleteShader(fragmentShader.id);
		glDeleteShader(vertexShader.id);
		glDeleteProgram(id);
	};
	bool isLinked() const noexcept { return linked; }; 
	void Link();
	
	GLuint id;
	Shader fragmentShader;
	Shader vertexShader;

private:
	bool linked;
}