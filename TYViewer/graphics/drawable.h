#pragma once

#include "shader.h"

class Drawable
{
public:
	virtual void draw(Shader& shader) const = 0;
};