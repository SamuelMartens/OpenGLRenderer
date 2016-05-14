#include "gl_core_4_3.h"

#include <string>
#include <cstring>
#include <iostream>

#include "model.h"
#include "Light.h"

Light::Light() :
direction(0, 0, 0),
type(LighType::None),
shiness(10),
coneAngle(90),
intensityAmbient(0.02),
intensityDiffuse(0.5),
intensitySpecular(0.75)
{
	model.LoadModel("E:\\C++\\OpenGLtutorial\\resources\\sphere.obj");
	model.type = Model::Type::lightModel;
	NormalizeLightModel();
};

Light::Light(glm::vec4& pos, glm::vec3& dir, LighType t) :
	direction(dir),
	type(t),
	shiness(10),
	coneAngle(90),
	intensityAmbient(0.02),
	intensityDiffuse(0.5),
	intensitySpecular(0.75)
{
	model.LoadModel("E:\\C++\\OpenGLtutorial\\resources\\sphere.obj");
	model.type = Model::Type::lightModel;
	model.position = pos;
	NormalizeLightModel();
};

void Light::NormalizeLightModel()
{
	model.scale = 0.02;
	model.Ka = glm::vec3(50);
}

void Light::PassToShaderProgram(GLuint progrmaId, int lightInd) const
{
	char lightArrayIndexInStr[100];

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].type", lightInd);
	GLint typeLoc = glGetUniformLocation(progrmaId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].position", lightInd);
	GLint positionLoc = glGetUniformLocation(progrmaId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].direction", lightInd);
	GLint directionLoc = glGetUniformLocation(progrmaId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].intensityAmbient", lightInd);
	GLint intensityAmbientLoc = glGetUniformLocation(progrmaId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].intensitySpecular", lightInd);
	GLint intensitySpecularLoc = glGetUniformLocation(progrmaId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].intensityDiffuse", lightInd);
	GLint intensityDiffuseLoc = glGetUniformLocation(progrmaId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].shiness", lightInd);
	GLint shinessLoc = glGetUniformLocation(progrmaId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].coneAngle", lightInd);
	GLint coneAngleLoc = glGetUniformLocation(progrmaId, lightArrayIndexInStr);

	sprintf_s(lightArrayIndexInStr, "lightSources[%d].coneShiness", lightInd);
	GLint coneShinessLoc = glGetUniformLocation(progrmaId, lightArrayIndexInStr);

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
