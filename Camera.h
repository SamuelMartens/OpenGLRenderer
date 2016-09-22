#pragma once

#include <memory>
#include <cassert>

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtx/vector_query.hpp"

#include "Viewport.h"
#include "ext_glm.h"

class Camera
{
public:
	Camera() :
		  position(0)
		, rotation(glm::vec3(0, 0, 0))
		, viewMat(ext_glm::IdentityMat())
		, viewport(std::make_unique<Viewport>())
		, updated(false)
	{};
	Camera(const glm::vec3& newPos, const glm::vec3& newRotation, std::unique_ptr<Viewport>&& newVPort) :
		position(newPos)
		, rotation(newRotation)
		, viewMat(ext_glm::IdentityMat())
		, viewport(std::move(newVPort))
		, updated(false)
	{};
	Camera(const Camera&) = delete;
	void operator=(const Camera&) = delete;
	~Camera() = default;

	/* Getters */
	glm::vec3 GetPosition() const noexcept { return position; };
	glm::vec3 GetRotation() const noexcept { return rotation; };
	const std::unique_ptr<Viewport>& GetViewport() const noexcept { return viewport; };
	std::unique_ptr<Viewport>& GetViewport()
	{
		// Here to avoid code duplication for const and non const getter.
		// This code is madness but I trust Scott Meyer, as it is his advice :)
		return const_cast<std::unique_ptr<Viewport>&>(
			static_cast<const Camera&>(*this).GetViewport());
	};
	const glm::mat4& GetViewMat()
	{
		if (!updated)
			CalculateViewMatrix();
		return viewMat;
	}
	glm::mat4 GetViewProjMat()
	{
		assert(viewport.get());
		// DEBUG swap
		return viewport->GetProjectionMat() * GetViewMat();
		//return GetViewMat();
	}

		
	/* Setters */
	void SetRotation(const glm::vec3& newRotation)
	{
		rotation = newRotation;
		updated = false;
	};
	void SetPosition(const glm::vec3& newPos) noexcept
	{ 
		position = newPos;
		updated = false;
	};
	void SetViewport(std::unique_ptr<Viewport>&& newVp) noexcept { viewport.reset(newVp.get()); };

	bool IsUpdated() const noexcept { return updated; };

private:

	void CalculateViewMatrix()
	{
		viewMat = ext_glm::move(-position) *  ext_glm::rotateXYZ(-rotation);
		updated = true;
	};
	
	glm::vec3 position;
	glm::vec3 rotation;
	
	bool updated;
	
	glm::mat4 viewMat;

	std::unique_ptr<Viewport> viewport;
};

