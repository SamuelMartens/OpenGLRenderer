#include "Graphic.h"
#include "shaders.h"
#include "ext_glm.h"
#include "model.h"
#include "Settings.h"

#include "gl_core_4_3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>
#include <iostream>
#include <string>
#include <cassert>
#include <memory>

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

Graphic::Renderer::Renderer(Settings& set) :
	vertShader(0),
	fragShader(0),
	shaderProgram(0),
	transMatLoc(-1),
	settings(set)
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
	
	std::string vertSource;
	std::string fragSource;

	/*  Choosing of shader */
	switch (settings.shading)
	{
	case (Settings::ShadingType::Flat):
		vertSource = Shaders::flatLightVS;
		fragSource = Shaders::flatLightFS;
		break;
	case (Settings::ShadingType::Guro) :
		vertSource = Shaders::guroLightVS;
		fragSource = Shaders::guroLightFS;
		break;
	case (Settings::ShadingType::Phong) :
		vertSource = Shaders::phongLightVS;
		fragSource = Shaders::phongLightFS;
		break;
	default:
		break;
	}

	const GLchar* vertSourceArray[] = { static_cast<const GLchar*> (vertSource.c_str()) };
	const GLchar* fragSourceArray[] = { static_cast<const GLchar*> (fragSource.c_str())};

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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return 0;
}

void Graphic::Renderer::Draw(float angle)
{


	for (auto& model: models)
	{
		model.LoadModelUniforms(shaderProgram);
		model.Draw();
		model.slopeAngle.y = angle;
		model.CalculateTransformMat();
		SetTransMatrix(model.transformMat);
	}
}


void Graphic::Renderer::Reload(float angle)
{

}

int Graphic::Renderer::InitUniforms()
{
	transMatLoc = glGetUniformLocation(shaderProgram, "trans");

	if (-1 == transMatLoc)
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

void Graphic::Renderer::AddModel(const Model& m)
{
	models.push_back(m);
	models.back().LoadGlData();
}

void Graphic::Renderer::AddLight(const Light&& l) 
{
	lights.push_back(l);
	AddModel(lights.back().GetModel());
}

void Graphic::Renderer::AddLight(const Light& l)
{
	lights.push_back(l);
	AddModel(lights.back().GetModel());
}

void Graphic::Renderer::AddLight(std::unique_ptr<Light> l)
{
	lights.push_back(*l);
	AddModel(lights.back().GetModel());
}

void Graphic::Renderer::LoadLightDataToOpenGL() const
{
	assert(lights.size() < settings.GetMaxLightNumber());
	for (unsigned i = 0; i < lights.size(); ++i)
		lights[i].PassToShaderProgram(shaderProgram, i);

	GLuint lightNumberLoc = glGetUniformLocation(shaderProgram, "lightSourcesNumber");
	glUniform1i(lightNumberLoc, lights.size());
}