#include <string>
#include <fstream>
#include <iostream>

#include "Shader.h"

#include "gl_core_4_3.h"

std::string Shader::ParseSourceFile(const std::string&&  filePath) const
{
	std::string shaderCode;
	std::ifstream shaderStream(filePath.c_str(), std::ios::in);
	if (shaderStream.is_open())
	{
		std::string line;
		while (std::getline(shaderStream, line))
			shaderCode += line + '\n';
		shaderStream.close();
	}
	return shaderCode;
}

void Shader::Compile(std::string&& shaderSource)
{
	switch(type)
	{
	case (Type::Vertex):
		id = glCreateShader(GL_VERTEX_SHADER);
		break;
	case (Type::Fragment):
		id = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	default:
		std::cout<<"Failed to compile shader. No type \n";
		return;
	}
	
	if (0 == id)
	{
		std::cout<< "Failed to create shader \n";
		return;
	}
	
	const GLchar* sourceArray[] = { static_cast<const GLchar*> (shaderSource.c_str())};
	glShaderSource(id, 1, sourceArray, NULL);
	
	glCompileShader(id);
	
	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result)
	{
		std::cout << "Failed to compile shader result. \n";
		
		GLint loglen;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &loglen);

		if (loglen > 0)
		{
			char* log = new char[loglen];

			GLsizei written;
			glGetShaderInfoLog(id, loglen, &written, log);
			std::cout << log << " \n";
			delete [] log;
		}
		return;
	}

	compiled = true;
}