#pragma once

#include <glm/trigonometric.hpp>

namespace Neptune
{
	float Degrees(float a)
	{
		return glm::degrees(a);
	}

	float Radians(float a)
	{
		return glm::radians(a);
	}
}