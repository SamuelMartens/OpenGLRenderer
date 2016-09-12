#pragma once

#include "glm\mat4x4.hpp"
#include "ext_glm.h"

class Viewport
{
public:
	Viewport() :
		  near(0)
		, far(0)
		, aspect(0)
		, fov(0)
		, updated(false)
	{
		InitProjectionMat();
	};
	Viewport(float newNear, float newFar, float newAspect, float newFow) :
		  updated(false)
		, near(newNear)
		, far(newFar)
		, aspect(newAspect)
		, fov(newFow)
	{
		InitProjectionMat();
	};
	~Viewport() = default ;

	/* Gettert */
	float GetNear() const noexcept { return near; };
	float GetFar() const noexcept { return far; };
	float GetAspect() const noexcept { return aspect; };
	float GetFov() const noexcept { return fov; };
	glm::mat4 GetProjectionMat() noexcept
	{
		if (!updated)
			CalculateProjectionMat();

		return projectionMat;
	};

	/* Setter */
	void SetNear(float val) noexcept { near = val; updated = false; };
	void SetFar(float val) noexcept { far = val; updated = false; };
	void SetAspect(float val) noexcept { aspect = val; updated = false; };
	void SetFov(float val) noexcept { fov = val; updated = false; };

	bool IsUpdated() const noexcept { return updated; };

private:
	void CalculateProjectionMat();
	void InitProjectionMat() noexcept
	{
		projectionMat = ext_glm::IdentityMat();
		projectionMat[3][3] = 0;
		projectionMat[3][2] = projectionMat[2][3] = 1;
	}

	float near;
	float far;
	// Aspect ratio
	float aspect;
	// Field of view for Y axis
	float fov;

	bool updated;
	glm::mat4 projectionMat;
};

