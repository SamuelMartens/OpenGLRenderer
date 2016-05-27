#include "ShaderProgram.h"
#include "Shader.h"

#include "gl_core_4_3.h"

#include <iostream>
#include <cassert>

void ShaderProgram::Link()
{
	assert(vertexShader.isCompiled() && fragmentShader.isCompiled());
	assert(Shader::Type::Vertex == vertexShader.type && Shader::Type::Fragment == fragmentShader.type);
	
	glAttachShader(id, vertexShader.id);
	glAttachShader(id, fragmentShader.id);
	
	glLinkProgram(id);
	
	GLint status;
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if ( GL_FALSE == status)
	{
		std::cout << "Failed to link shader progrma. \n";

		GLint loglen;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &loglen);
		if (loglen > 0)
		{
			char* log = new char[loglen];
			GLsizei written;
			glGetProgramInfoLog(id, loglen, &written, log);
			std::cout << log << ". \n";
			delete[] log;
		}

		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(id);
	
	linked = true;
	
	return;
}

ShaderProgram::ShaderProgram(Shader& vs, Shader& fs):
	  vertexShader(vs)
	, fragmentShader(fs)
{
	assert(Shader::Type::Vertex == vertexShader.type && Shader::Type::Fragment == fragmentShader.type);
	assert(vertexShader.isCompiled() && fragmentShader.isCompiled());
	
	id = glCreateProgram();
	if (0 == id)
		std::cout << "Failed to create shader program \n";

	Link();
}

ShaderProgram::ShaderProgram(Shader&& vs, Shader&& fs) :
	vertexShader(vs)
	, fragmentShader(fs)
{
	assert(Shader::Type::Vertex == vertexShader.type && Shader::Type::Fragment == fragmentShader.type);
	assert(vertexShader.isCompiled() && fragmentShader.isCompiled());

	id = glCreateProgram();
	if (0 == id)
		std::cout << "Failed to create shader program \n";

	Link();
}