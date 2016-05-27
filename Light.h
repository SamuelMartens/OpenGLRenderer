#pragma once

#include "glm/glm.hpp"

#include "model.h"

class Light
{
public:

	enum class LighType
	{
		None,
		PointLight,
		DirectionLight,
		ConeLigh
	};

	Light();
	Light(glm::vec4& pos, glm::vec3& dir, LighType t);
	Light(const Light& val) = default;
	Light(Light&& val) = default;
	~Light() = default;

	/*  Getters */
	glm::vec4 GetPosition() const noexcept { return model.position; };
	Model GetModel() const noexcept { return model; };
	glm::vec3 GetDirection() const noexcept { return direction; };


	/* Setters */
	void SetPosition(glm::vec4& newPos) noexcept { model.position = newPos; };
	void SetPosition(glm::vec4&& newPos) noexcept {model.position = newPos;};
	void SetDirection(glm::vec3& newDir) noexcept { direction = glm::normalize(newDir); };
	void SetDirection(glm::vec3&& newDir ) noexcept{ direction = glm::normalize(newDir); };

	/* Other functions */
	void PassToShaderProgram(GLuint programId, int lightInd=0) const;

	/* General data */
	
	LighType type;
	float shiness;
	float coneAngle;
	float coneShiness;
	glm::vec3 intensityAmbient;
	glm::vec3 intensityDiffuse;
	glm::vec3 intensitySpecular;

private:
	void NormalizeLightModel();
	Model  model;
	glm::vec3 direction;
};

