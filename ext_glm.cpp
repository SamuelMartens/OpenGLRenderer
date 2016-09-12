#include "ext_glm.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cmath>

glm::mat4 ext_glm::rotateY(const float angle)
{
	float rotateMatrix[16] =
	{
		std::cos(angle), 0, std::sin(angle), 0,
		0, 1, 0, 0,
		-std::sin(angle), 0, std::cos(angle), 0,
		0,               0, 0, 1
	};
	return glm::make_mat4(rotateMatrix);
}

glm::mat4 ext_glm::rotateX(const float angle)
{
	float rotateMatrix[16] =
	{
		1,  0,  0,  0,
		0,  std::cos(angle), std::sin(angle), 0,
		0,  -std::sin(angle),  std::cos(angle), 0,
		0,  0,  0,  1
	}; 

	return glm::make_mat4(rotateMatrix);
}

glm::mat4 ext_glm::rotateZ(const float angle)
{
	float rotateMatrix[16] =
	{
		std::cos(angle), std::sin(angle), 0, 0,
		-std::sin(angle), std::cos(angle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	return glm::make_mat4(rotateMatrix);
}

glm::mat4 ext_glm::move(const float x, const float y, const float z)
{
	float moveMatrix[16] =
	{
		1,  0,  0,  0,
		0,  1,  0,  0,
		0,  0,  1,  0,
		x,  y,  z,  1
	};

	return glm::make_mat4(moveMatrix);
}

glm::mat4 ext_glm::move(const glm::vec3& moveVec)
{
	float moveMatrix[16] =
	{
		1,  0,  0,  0,
		0,  1,  0,  0,
		0,  0,  1,  0,
		moveVec.x, moveVec.y, moveVec.z, 1
	};

	return glm::make_mat4(moveMatrix);
}

glm::mat4 ext_glm::scale(const float x, const float y, const float z)
{
	float scaleMatrix[16] =
	{
		x,  0,  0,  0,
		0,  y,  0,  0,
		0,  0,  z,  0,
		0,  0,  0,  1
	};

	return glm::make_mat4(scaleMatrix);
}

glm::mat4 ext_glm::scale(const float n)
{
	float scaleMatrix[16] =
	{
		n,  0,  0,  0,
		0,  n,  0,  0,
		0,  0,  n,  0,
		0,  0,  0,  1
	};

	return glm::make_mat4(scaleMatrix);
}

glm::mat4 ext_glm::IdentityMat() noexcept
{
	float identityMatrix[16] =
	{
		1,  0,  0,  0,
		0,  1,  0,  0,
		0,  0,  1,  0,
		0,  0,  0,  1
	};

	return glm::make_mat4(identityMatrix);
}