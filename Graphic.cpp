#include "Graphic.h"
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

Graphic::Renderer::Renderer() :
	  transMatLoc(-1)
{};

int Graphic::Renderer::Init() 
{
	if (!shaderProgram.isLinked())
	{
		std::cout<<"Shader program is not linked. Failed to init renderer \n";
		return 1;
	}
	Settings::Instance().resources.Init(shaderProgram);
	Settings::Instance().PassToShaderProgram(shaderProgram);
	
	if (InitUniforms() != 0)
		std::cout << "Failed to load uniforms \n";

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return 0;
}

void Graphic::Renderer::Draw(float angle)
{
	for (unsigned i = 0; i < models.size(); ++i)
	{
		
		ActivateAppropriteToModelSubroutines(models[i]);

		models[i].slopeAngle.y = angle;
		models[i].CalculateTransformMat();
		SetTransMatrix(models[i].transformMat);
		models[i].LoadModelUniforms(shaderProgram);
		models[i].Draw(Settings::Instance().resources);
		
	}
}


void Graphic::Renderer::Reload(float angle)
{

}

int Graphic::Renderer::InitUniforms()
{
	transMatLoc = glGetUniformLocation(shaderProgram.id, "trans");
	modelSubroutine = glGetSubroutineIndex(shaderProgram.id, GL_FRAGMENT_SHADER, "PhongLight");
	lightSubroutine = glGetSubroutineIndex(shaderProgram.id, GL_FRAGMENT_SHADER, "LighSourceLight");
	normalTextureSubroutines[0] = glGetSubroutineIndex(shaderProgram.id, GL_FRAGMENT_SHADER, "TransformToObjectLocal");
	normalTextureSubroutines[1] = glGetSubroutineIndex(shaderProgram.id, GL_FRAGMENT_SHADER, "GetNormalFromTexture");
	noNormalTextureSubroutines[0] = glGetSubroutineIndex(shaderProgram.id, GL_FRAGMENT_SHADER, "NotTransformToObjectLocal");
	noNormalTextureSubroutines[1] = glGetSubroutineIndex(shaderProgram.id, GL_FRAGMENT_SHADER, "GetOriginalNormal");

	if (   GL_INVALID_INDEX == modelSubroutine
		|| GL_INVALID_INDEX == lightSubroutine
		|| GL_INVALID_INDEX == normalTextureSubroutines[0]
		|| GL_INVALID_INDEX == normalTextureSubroutines[1]
		|| GL_INVALID_INDEX == noNormalTextureSubroutines[0]
		|| GL_INVALID_INDEX == noNormalTextureSubroutines[1])
		std::cout << "Failed to get graphic subroutine index. \n ";

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
	LoadLightDataToOpenGL();
}

void Graphic::Renderer::AddLight(const Light& l)
{
	lights.push_back(l);
	AddModel(lights.back().GetModel());
	LoadLightDataToOpenGL();
}

void Graphic::Renderer::AddLight(std::unique_ptr<Light> l)
{
	lights.push_back(*l);
	AddModel(lights.back().GetModel());
	LoadLightDataToOpenGL();
}

void Graphic::Renderer::LoadLightDataToOpenGL() const
{
	assert(lights.size() <= Settings::Instance().GetMaxLightNumber());
	for (unsigned i = 0; i < lights.size(); ++i)
		lights[i].PassToShaderProgram(shaderProgram.id, i);

	GLuint lightNumberLoc = glGetUniformLocation(shaderProgram.id, "lightSourcesNumber");
	glUniform1i(lightNumberLoc, lights.size());
}

void Graphic::Renderer::ActivateAppropriteToModelSubroutines(const Model& model) const
{
	constexpr unsigned numOfUsedSubroutines = 3;
	GLuint subroutinesArray[numOfUsedSubroutines];

	const GLint shadeSubUniformLoc = glGetSubroutineUniformLocation(shaderProgram.id, GL_FRAGMENT_SHADER, "shadeModel");
	const GLint toObjectLocalCoordSubUniformLoc = glGetSubroutineUniformLocation(shaderProgram.id, GL_FRAGMENT_SHADER, "toObjectLocalCoord");
	const GLint getNormalVecSubUniformLoc = glGetSubroutineUniformLocation(shaderProgram.id, GL_FRAGMENT_SHADER, "getNormalVec");

	assert(shadeSubUniformLoc < numOfUsedSubroutines
		&& toObjectLocalCoordSubUniformLoc < numOfUsedSubroutines
		&& getNormalVecSubUniformLoc < numOfUsedSubroutines);

	if (   -1 == shadeSubUniformLoc
		|| -1 == toObjectLocalCoordSubUniformLoc
		|| -1 == getNormalVecSubUniformLoc)
	{
		std::cout << "Failed to get subroutines uniform location. \n";
		return;
	}

	switch (model.type)
	{
	case Model::Type::commonModel:
		subroutinesArray[shadeSubUniformLoc] = modelSubroutine;
		break;
	case Model::Type::lightModel:
		subroutinesArray[shadeSubUniformLoc] = lightSubroutine;
		break;
	default:
		break;
	}

	if (model.material.HasTextureWithType(Texture::Type::Normal))
	{
		subroutinesArray[toObjectLocalCoordSubUniformLoc] = normalTextureSubroutines[0];
		subroutinesArray[getNormalVecSubUniformLoc] = normalTextureSubroutines[1];
	}
	else
	{
		subroutinesArray[toObjectLocalCoordSubUniformLoc] = noNormalTextureSubroutines[0];
		subroutinesArray[getNormalVecSubUniformLoc] = noNormalTextureSubroutines[1];
	}

	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 3, subroutinesArray);
}
