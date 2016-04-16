#include "Graphic.h"
#include "shaders.h"
#include "ext_glm.h"
#include "model.h"

#include "gl_core_4_3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>
#include <iostream>

void Graphic::InitFigure(std::vector<float>& vertices, std::vector<float>& colors)
{
	vertices = 
	{
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,

		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,

		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,

		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,

		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,

		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,

		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,

		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f
	};

	colors =
	{
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 1.1f, 0.0f,
		0.0f, 1.1f, 0.0f,
		0.0f, 1.1f, 0.0f,

		0.0f, 1.1f, 0.0f,
		0.0f, 1.1f, 0.0f,
		0.0f, 1.1f, 0.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};
	
}

Graphic::Renderer::Renderer() :
	vertShader(0),
	fragShader(0),
	shaderProgram(0),
	transMatLoc(-1),
	lightPosition(0, 0, 0, 1)
{};

Graphic::Renderer::~Renderer()
{
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

	const GLchar* vertSourceArray[] = { Shaders::diffuseLightVS };
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
	const GLchar* fragSourceArray[] = { Shaders::diffuseLightFS };
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

	/* Ligth source position */
	SetLightPostion(0.0, 0.7, -1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return 0;
}

void Graphic::Renderer::Draw(float angle)
{
	glm::mat4 trMat = ext_glm::rotateX(-0.5) * ext_glm::rotateY(angle) * ext_glm::scale(0.5);


	for (auto& model: models)
	{
		model.Draw();
		model.transformMat = trMat;
		SetTransMatrix(model.transformMat);
	}
}


void Graphic::Renderer::Reload(float angle)
{

}

int Graphic::Renderer::InitUniforms()
{
	transMatLoc = glGetUniformLocation(shaderProgram, "trans");
	lightPosLoc = glGetUniformLocation(shaderProgram, "lightPosition");

	if (-1 == transMatLoc || -1 == lightPosLoc)
		return -1;

	return 0;
}

void Graphic::Renderer::SetTransMatrix(glm::mat4 &transMat)
{
	glUniformMatrix4fv(transMatLoc, 1, GL_FALSE, &transMat[0][0]);
}

void Graphic::Renderer::ClearScreen() const
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphic::Renderer::AddModel(const Model&& m)
{
	models.push_back(m);
	models.back().LoadGlData();
}

void Graphic::Renderer::SetLightPostion(float x, float y, float z)
{
	lightPosition = glm::vec4(x, y, z, 1);
	glUniform4fv(lightPosLoc, 1, &lightPosition[0]);
}