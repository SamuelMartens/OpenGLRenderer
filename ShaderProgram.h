#pragma once

#include <iostream>

#include "Shader.h"

#include "gl_core_4_3.h"

class ShaderProgram
{
public:

	enum class Type
	{
		Main
	};

	explicit ShaderProgram(Type t):
		  type(t)
		, linked(false)
	{
		id  = glCreateProgram();
		if (0 == id)
			std::cout<<"Failed to create shader program \n";
	};
	ShaderProgram(Shader& vs, Shader& fs , Type t);
	ShaderProgram(Shader&& vs, Shader&& fs, Type t);
	
	~ShaderProgram()
	{
		glDeleteShader(fragmentShader.id);
		glDeleteShader(vertexShader.id);
		glDeleteProgram(id);
	};
	void Link();

	/* Getters */
	bool isLinked() const noexcept { return linked; };
	Type GetType() const noexcept { return type; };
	
	GLuint id;
	Shader fragmentShader;
	Shader vertexShader;

private:
	bool linked;
	Type type;
};