#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(const glm::vec3& position, const glm::vec3& rotation,
		float fieldOfView, float aspectRatio, float clipNear = 0.1f, float clipFar = 8000.0f);

public:
	void setPosition(const glm::vec3& p);
	const glm::vec3& getPosition() const;

	void setRotation(const glm::vec3& r);
	const glm::vec3& getRotation() const;

	void setFieldOfView(const float& fov);
	const float& getFieldOfView() const;

	void setAspectRatio(const float& ar);
	const float& getAspectRatio() const;

	void setClipPlaneNear(const float& n);
	const float& getClipPlaneNear() const;

	void setClipPlaneFar(const float& f);
	const float& getClipPlaneFar() const;

	const glm::mat4& getProjectionMatrix() const;
	const glm::mat4& getViewMatrix() const;

public:
	void localRotate(const glm::vec3& r);
	void localTranslate(const glm::vec3& t);

private:
	void updateProjectionMatrix();
	void updateViewMatrix();

private:
	float fieldOfView;
	float aspectRatio;

	float clipPlaneNear;
	float clipPlaneFar;

private:
	glm::vec3 position;
	glm::vec3 rotation;

	glm::vec3 localForward;
	glm::vec3 localUp;
	glm::vec3 localRight;

	glm::vec3 worldUp;

	glm::mat4 projection;
	glm::mat4 view;
};