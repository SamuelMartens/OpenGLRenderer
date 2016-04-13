#pragma once

#include "glm/glm.hpp"

namespace ext_glm
{
	glm::mat4 rotateZ(float angle);
	glm::mat4 rotateX(float angle);
	glm::mat4 move(float x, float y=0, float z=0);
	glm::mat4 scale(float x, float y, float z);
	glm::mat4 scale(float n);
}