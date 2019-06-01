#pragma once

#include <string>

class Texture
{
public:
	Texture(unsigned int id);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;
private:
	unsigned int id;
};