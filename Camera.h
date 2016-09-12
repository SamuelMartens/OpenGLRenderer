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
		, direction(glm::vec3(0, 0, 1))
		, viewMat(ext_glm::IdentityMat())
		, viewport(std::make_unique<Viewport>())
		, updated(false)
	{};
	Camera(const glm::vec3& newPos, const glm::vec3& newDir, std::unique_ptr<Viewport>&& newVPort) :
		position(newPos)
		, direction(newDir)
		, viewMat(ext_glm::IdentityMat())
		, viewport(newVPort.get())
		, updated(false)
	{
		assert(glm::isNormalized(newDir, 0.0002f));
	};
	~Camera() = default;

	/* Getters */
	glm::vec3 GetPosition() const noexcept { return position; };
	glm::vec3 GetDirection() const noexcept { return direction; };
	const std::unique_ptr<Viewport>& GetViewport() const noexcept { return viewport; };
	std::unique_ptr<Viewport>& GetViewport() noexcept { return viewport; };
	
	/* Setters */
	void SetDirection(const glm::vec3& newDir)
	{
		assert(glm::isNormalized(newDir, 0.0002f));
		direction = newDir;
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
	void CalculateViewMatrix();
	
	glm::vec3 position;
	glm::vec3 direction;
	
	bool updated;
	
	glm::mat4 viewMat;

	std::unique_ptr<Viewport> viewport;
};

