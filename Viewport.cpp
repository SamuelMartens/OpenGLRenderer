#include "Viewport.h"

#include <cmath>
#include <cassert>

#include "glm\glm.hpp"

void Viewport::CalculateProjectionMat()
{
	const float fov_ctg = 1 / std::tan(fov / 2);

	assert(aspect && 0 < vpNear && vpFar > vpNear);
	projectionMat[0][0] = fov_ctg / aspect;
	projectionMat[1][1] = fov_ctg;
	projectionMat[2][2] = (vpFar + vpNear) / (vpFar - vpNear);
	projectionMat[3][2] = -2 * vpFar * vpNear / (vpFar - vpNear);

	updated = true;
}
