#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 position, float fieldOfView, float aspectRatio, float near = 0.1f, float far = 10000.0f);

	inline void setFieldOfView(float fov)	{ m_fieldOfView = fov; }
	inline void setAspectRatio(float ratio) { m_aspectRatio = ratio; }

	void localTranslate(const glm::vec3& translation);
	void localRotate(const glm::vec2& rotation);

	const glm::mat4 getViewProjection() const;
	const glm::mat4 getViewMatrix() const;

private:
	void update();

	float m_fieldOfView;
	float m_aspectRatio;
	
	float m_near;
	float m_far;

	float m_speed = 200.0f;

	float m_sensitivityX = 0.1f;
	float m_sensitivityY = 0.1f;

	float m_rotationX = -90.0f;;
	float m_rotationY = 0.0f;

	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 m_right;

	glm::vec3 m_worldUp;
};