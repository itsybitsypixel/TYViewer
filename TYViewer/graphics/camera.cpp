#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, float fieldOfView, float aspectRatio, float near, float far) :
	m_position(position),
	m_fieldOfView(fieldOfView), m_aspectRatio(aspectRatio), 
	m_near(near), m_far(far),
	m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f))
{
	update();
}

void Camera::localTranslate(const glm::vec3& translation)
{
	m_position += ((-m_right * translation.x) + (m_up * translation.y) + (m_forward * translation.z)) * m_speed;
}
void Camera::localRotate(const glm::vec2& rotation)
{
	m_rotationX += rotation.x * m_sensitivityX;
	m_rotationY += rotation.y * m_sensitivityY;

	update();
}

const glm::mat4 Camera::getViewProjection() const
{
	return glm::perspective(glm::radians(m_fieldOfView), m_aspectRatio, m_near, m_far);
}
const glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_position, m_position + m_forward, m_up);
}

void Camera::update()
{
	m_forward.x = cos(glm::radians(m_rotationX)) * cos(glm::radians(m_rotationY));
	m_forward.y = sin(glm::radians(m_rotationY));
	m_forward.z = sin(glm::radians(m_rotationX)) * cos(glm::radians(m_rotationY));
	m_forward = glm::normalize(m_forward);

	m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
	m_up	= glm::normalize(glm::cross(m_right, m_forward));
}
