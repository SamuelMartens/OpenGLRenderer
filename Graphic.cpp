#include "Graphic.h"
#include "shaders.h"
#include "ext_glm.h"

#include "gl_core_4_3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>
#include <iostream>

void Graphic::InitFigure(std::vector<float>& vertices, std::vector<float>& colors)
{
	vertices = 
	{
		0.0f, 0.8f, 0.0f,
		-0.4f, -0.4f, 0.0f,
		0.4f, -0.4f, 0.0f
	};

	colors =
	{
		1, 0, 0,
		0 ,1 , 0,
		0, 0, 1
	};
	
}

Graphic::Renderer::Renderer() :
	vertexArrayBuffer(0),
	vertShader(0),
	fragShader(0),
	shaderProgram(0)
{};

Graphic::Renderer::~Renderer()
{
	glDeleteFramebuffers(1, &vertexArrayBuffer);
	glDeleteShader(fragShader);
	glDeleteShader(vertShader);
	glDeleteProgram(shaderProgram);
};

int Graphic::Renderer::InitShaders()
{
	/* Compile shaders */

	vertShader = glCreateShader(GL_VERTEX_SHADER);
	if (0 == vertShader)
	{
		std::cout << "Failed to load shaders. \n";
		return 1;
	}

	const GLchar* vertSourceArray[] = { Shaders::vertextShader };
	glShaderSource(vertShader, 1, vertSourceArray, NULL);

	glCompileShader(vertShader);
	
	GLint vertRes;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &vertRes);
	if (GL_FALSE == vertRes)
	{
		std::cout << "Failed to compile vertex shader. \n";
		
		GLint loglen;
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &loglen);

		if (loglen > 0)
		{
			char* log = new char[loglen];

			GLsizei written;
			glGetShaderInfoLog(vertShader, loglen, &written, log);
			std::cout << log << " \n";
			delete [] log;
		}
		return 1;
	}

	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSourceArray[] = { Shaders::fragmentShader };
	glShaderSource(fragShader, 1, fragSourceArray, NULL);

	if (0 == fragShader)
	{
		std::cout << "Failed to load shaders. \n";
		return 1;
	}

	glCompileShader(fragShader);

	GLint fragRes;
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragRes);

	if (GL_FALSE == fragRes)
	{
		std::cout << "Failed to compile frag shader. \n";

		GLint loglen;
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &loglen);

		if (loglen > 0)
		{
			char* log = new char[loglen];

			GLsizei written;
			glGetShaderInfoLog(fragShader, loglen, &written, log);
			std::cout << log << " \n";
			delete[] log;
		}
		return 1;
	}

	/* Create and link shader program */
	shaderProgram = glCreateProgram();
	if (0 == shaderProgram) 
	{
		std::cout << "Failed to create shader program \n";
		return 1;
	}

	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);

	glLinkProgram(shaderProgram);

	GLint status;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if ( GL_FALSE == status)
	{
		std::cout << "Failed to link shader progrma. \n";

		GLint loglen;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &loglen);
		if (loglen > 0)
		{
			char* log = new char[loglen];
			GLsizei written;
			glGetProgramInfoLog(shaderProgram, loglen, &written, log);
			std::cout << log << ". \n";
			delete[] log;
		}

		return 1;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram);

	return 0;
}

int Graphic::Renderer::Init() 
{
	if (InitShaders() != 0)
		return 1;

	if (InitUniforms() !=0)
		std::cout << "Failed to load uniforms \n";


	/* Work with vertex buffer array */
	glGenBuffers(1, &verticesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_DYNAMIC_DRAW);

	/* Work with vertex buffer array */
	glGenVertexArrays(1, &vertexArrayBuffer);
	glBindVertexArray(vertexArrayBuffer);

	glEnableVertexAttribArray(static_cast<GLint>(VertexAtrib::VertexCoors));
	glEnableVertexAttribArray(static_cast<GLint>(VertexAtrib::Colors));

	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glVertexAttribPointer(static_cast<GLint>(VertexAtrib::VertexCoors), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(static_cast<GLint>(VertexAtrib::Colors), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(vertexArrayBuffer);

	return 0;
}

void Graphic::Renderer::Draw()
{
	glDrawArrays(GL_TRIANGLES, 0, 3);
}


void Graphic::Renderer::Reload()
{
	glBindVertexArray(vertexArrayBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(static_cast<GLint>(VertexAtrib::VertexCoors), 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

int Graphic::Renderer::InitUniforms()
{
	transMat = glGetUniformLocation(shaderProgram, "trans");
	if (-1 == transMat)
		return -1;

	return 0;
}

void Graphic::Renderer::SetTransMatrix(glm::mat4 &transMat)
{
	glUniformMatrix4fv(shaderProgram, 1, GL_FALSE, &transMat[0][0]);
}