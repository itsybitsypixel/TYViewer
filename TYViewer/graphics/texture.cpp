#include "texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SOIL/SOIL2.h"

Texture::Texture(const std::string& filepath)
	: m_id(0)
{
	m_id = SOIL_load_OGL_texture(filepath.c_str(), 0, 0, SOIL_FLAG_INVERT_Y);

	// Replaced by soil2
	//glGenTextures(1, &m_id);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_localbuffer);

	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

void Texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}
void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
