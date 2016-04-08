#include "ext_glm.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cmath>

glm::mat4 ext_glm::rotateZ(float angle)
{
	float rotateMatrix[16] =
	{
		std::cos(angle), 0, -1 * std::sin(angle), 0,
		std::sin(angle), 0, std::cos(angle), 0,
		0,               0, 0, 1,
		0,               0, 0, 1
	};

	float rotateMatrix1[16] =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	}; 

	return glm::make_mat4(rotateMatrix1);
}