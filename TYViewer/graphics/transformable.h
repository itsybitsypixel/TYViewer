#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <glm/gtc/matrix_transform.hpp>

class Transformable
{
public:
	Transformable(const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
		: position(position), rotation(rotation), scale(scale)
	{}

	inline void setPosition(const glm::vec3& p) { position = p; }
	inline const glm::vec3& getPosition() const { return position; }

	inline void setRotation(const glm::vec3& r) { rotation = r; }
	inline const glm::vec3& getRotation() const { return rotation; }

	inline void setScale(const glm::vec3& s) { scale = s; }
	inline const glm::vec3& getScale() const { return scale; }


	inline const glm::mat4 getMatrix() const
	{
		glm::mat4 matrix(1.0f);
		matrix = glm::scale(matrix, scale);
		matrix = glm::translate(matrix, position);
		matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		return matrix;
	}

protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};