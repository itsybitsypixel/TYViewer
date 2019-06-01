#include "debug.h"

#include <iostream>

void Debug::log(const std::string& message)
{
	std::cout << message << std::endl;
}
