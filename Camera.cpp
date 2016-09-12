#include "Camera.h"

#include "ext_glm.h"

void Camera::CalculateViewMatrix()
{
	viewMat = ext_glm::move(-position) * 
	updated = true;
}