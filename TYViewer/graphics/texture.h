#pragma once

#include <string>

class Texture
{
public:
	Texture() = default;
	Texture(const std::string& filepath);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;
private:
	unsigned int m_id;
};