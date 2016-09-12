#include "Viewport.h"

#include <cmath>
#include <cassert>

#include "glm\glm.hpp"

void Viewport::CalculateProjectionMat()
{
	const float fov_ctg = 1 / std::tan(fov / 2);

	assert(aspect && near >= 0 && far > near);
	projectionMat[0][0] = fov_ctg / aspect;
	projectionMat[1][1] = fov_ctg;
	projectionMat[2][2] = (far + near) / (far - near);
	projectionMat[2][3] = -2 * far * near / (far - near);

	updated = true;
}
