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
#include <map>


Graphic::Renderer::Renderer() :
	  camera(nullptr)
	, skyBox(nullptr)
	, normalMatLoc(-1)
	, modelViewMatLoc(-1)
	, modelViewProjMatLoc(-1)
	, modelSubroutine(0)
	, lightSubroutine(0)
{
	normalTextureSubroutines[0] = normalTextureSubroutines[1] = 0;
	noNormalTextureSubroutines[0] = noNormalTextureSubroutines[1] = 0;
};

int Graphic::Renderer::Init() 
{
	if (!GetShaderProgramWithType(ShaderProgram::Type::Main)->isLinked())
	{
		std::cout<<"Shader program is not linked. Failed to init renderer \n";
		return 1;
	}
	Settings::Instance().resources.Init(*GetShaderProgramWithType(ShaderProgram::Type::Main));
	Settings::Instance().PassToShaderProgram(*GetShaderProgramWithType(ShaderProgram::Type::Main));
	
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
		SetTransMatrices(models[i].GetModelMat());
		models[i].LoadModelUniforms(*GetShaderProgramWithType(ShaderProgram::Type::Main));
		models[i].Draw(Settings::Instance().resources);
		
	}
}

int Graphic::Renderer::InitUniforms()
{
	modelViewProjMatLoc = glGetUniformLocation(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, "modelViewProjMat");
	modelViewMatLoc = glGetUniformLocation(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, "modelViewMat");
	normalMatLoc = glGetUniformLocation(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, "normalMat");
	modelSubroutine = glGetSubroutineIndex(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, GL_FRAGMENT_SHADER, "PhongLight");
	lightSubroutine = glGetSubroutineIndex(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, GL_FRAGMENT_SHADER, "LighSourceLight");
	normalTextureSubroutines[0] = glGetSubroutineIndex(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, GL_FRAGMENT_SHADER, "TransformToObjectLocal");
	normalTextureSubroutines[1] = glGetSubroutineIndex(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, GL_FRAGMENT_SHADER, "GetNormalFromTexture");
	noNormalTextureSubroutines[0] = glGetSubroutineIndex(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, GL_FRAGMENT_SHADER, "NotTransformToObjectLocal");
	noNormalTextureSubroutines[1] = glGetSubroutineIndex(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, GL_FRAGMENT_SHADER, "GetOriginalNormal");

	if (   GL_INVALID_INDEX == modelSubroutine
		|| GL_INVALID_INDEX == lightSubroutine
		|| GL_INVALID_INDEX == normalTextureSubroutines[0]
		|| GL_INVALID_INDEX == normalTextureSubroutines[1]
		|| GL_INVALID_INDEX == noNormalTextureSubroutines[0]
		|| GL_INVALID_INDEX == noNormalTextureSubroutines[1])
		std::cout << "Failed to get graphic subroutine index. \n ";

	if (-1 == modelViewProjMatLoc
		|| -1 == normalMatLoc
		|| -1 == modelViewMatLoc)
	{
		std::cout << "Failed to get trans matrices loc";
		return -1;
	}

	return 0;
}

void Graphic::Renderer::LoadLightDataToOpenGL() const
{
	assert(lights.size() <= Settings::Instance().GetMaxLightNumber());
	for (unsigned i = 0; i < lights.size(); ++i)
		lights[i].PassToShaderProgram(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, i);

	GLuint lightNumberLoc = glGetUniformLocation(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, "lightSourcesNumber");
	glUniform1i(lightNumberLoc, lights.size());
}

void Graphic::Renderer::ActivateAppropriteToModelSubroutines(const Model& model) const
{
	constexpr unsigned numOfUsedSubroutines = 3;
	GLuint subroutinesArray[numOfUsedSubroutines];

	const GLint shadeSubUniformLoc = glGetSubroutineUniformLocation(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, GL_FRAGMENT_SHADER, "shadeModel");
	const GLint toObjectLocalCoordSubUniformLoc = glGetSubroutineUniformLocation(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, GL_FRAGMENT_SHADER, "toObjectLocalCoord");
	const GLint getNormalVecSubUniformLoc = glGetSubroutineUniformLocation(GetShaderProgramWithType(ShaderProgram::Type::Main)->id, GL_FRAGMENT_SHADER, "getNormalVec");

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

void Graphic::Renderer::AddShaderProgram(ShaderProgram* newShaderProgram)
{
	assert(newShaderProgram);
	auto res = shaderPrograms.insert({ newShaderProgram->GetType(), newShaderProgram });

	if (res.second)
		return;

	delete res.first->second;
	res.first->second = newShaderProgram;
}