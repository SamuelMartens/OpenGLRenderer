#include "ext_glm.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cmath>

glm::mat3 ext_glm::rotateZ(float angle)
{
	float rotateMatrix[9] =
	{
		std::cos(angle), 0, -1 * std::sin(angle),
		std::sin(angle), 0, std::cos(angle),
		0,               0, 1
	};

	float rotateMatrix1[9] =
	{
		1, 1, 1,
		1, 1, 1,
		1, 1, 1
	};

	return glm::make_mat3(rotateMatrix1);
}