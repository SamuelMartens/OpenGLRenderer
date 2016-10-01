#include "gl_core_4_3.h"

#include <string>
#include <cstring>
#include <iostream>

#include "glm/glm.hpp"

#include "model.h"
#include "Light.h"
#include "Settings.h"

Light::Light() :
direction(0, 0, 0),
type(LighType::None),
shiness(10),
coneAngle(90),
intensityAmbient(0.02),
intensityDiffuse(0.5),
intensitySpecular(0.75)
{
	model.LoadModel(Settings::Instance().resources.GetPathSphereModel().c_str());
	model.SetType(Model::Type::lightModel);
	NormalizeLightModel();
};

Light::Light(glm::vec4& pos, glm::vec3& dir, LighType t) :
	direction(glm::normalize(dir)),
	type(t),
	shiness(10),
	coneAngle(90),
	intensityAmbient(0.02),
	intensityDiffuse(0.5),
	intensitySpecular(0.75)
{
	model.LoadModel(Settings::Instance().resources.GetPathSphereModel().c_str());
	model.SetType(Model::Type::lightModel);
	model.position = pos;
	NormalizeLightModel();
};

void Light::NormalizeLightModel()
{
	model.scale = 0.02;
}

void Light::PassToShaderProgram(GLuint programId, int lightInd) const
{
	char lightArrayIndexInStr[100];

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].type", lightInd);
	GLint typeLoc = glGetUniformLocation(programId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].position", lightInd);
	GLint positionLoc = glGetUniformLocation(programId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].direction", lightInd);
	GLint directionLoc = glGetUniformLocation(programId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].intensityAmbient", lightInd);
	GLint intensityAmbientLoc = glGetUniformLocation(programId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].intensitySpecular", lightInd);
	GLint intensitySpecularLoc = glGetUniformLocation(programId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].intensityDiffuse", lightInd);
	GLint intensityDiffuseLoc = glGetUniformLocation(programId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].shiness", lightInd);
	GLint shinessLoc = glGetUniformLocation(programId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].coneAngle", lightInd);
	GLint coneAngleLoc = glGetUniformLocation(programId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].coneShiness", lightInd);
	GLint coneShinessLoc = glGetUniformLocation(programId, lightArrayIndexInStr);

	if (-1 == typeLoc || -1 == positionLoc || -1 == directionLoc || -1 == intensityAmbientLoc
		|| -1 == intensitySpecularLoc || -1 == intensityDiffuseLoc || -1 == shinessLoc || -1 == coneAngleLoc || -1 == coneShinessLoc)
		std::cout << "Failed to get uniform location \n";


	glUniform1i(typeLoc, static_cast<GLint>(type));
	glUniform4fv(positionLoc, 1, &GetPosition()[0]);
	glUniform3fv(directionLoc, 1, &direction[0]);
	glUniform3fv(intensityAmbientLoc, 1, &intensityAmbient[0]);
	glUniform3fv(intensityDiffuseLoc, 1, &intensityDiffuse[0]);
	glUniform3fv(intensitySpecularLoc, 1, &intensitySpecular[0]);
	glUniform1f(shinessLoc, shiness);
	glUniform1f(coneAngleLoc, coneAngle);
	glUniform1f(coneShinessLoc, coneShiness);
}