#pragma once

#include "glm/glm.hpp"

namespace ext_glm
{
	/* Rotation */
	glm::mat4 rotateY(const float angle);
	glm::mat4 rotateX(const float angle);
	glm::mat4 rotateZ(const float angle);
	// Do all angles rotate here
	inline glm::mat4 rotateXYZ(const glm::vec3& rotateVec)
	{
		return rotateY(rotateVec.y) * rotateX(rotateVec.x) * rotateZ(rotateVec.z);
	};

	/* Translation */
	glm::mat4 move(const float x, const float y = 0, const float z = 0);
	glm::mat4 move(const glm::vec3& moveVec);
	glm::mat4 scale(const float x, const float y, const float z);
	glm::mat4 scale(const float n);

	/* Matrices */
	glm::mat4 IdentityMat() noexcept;
}