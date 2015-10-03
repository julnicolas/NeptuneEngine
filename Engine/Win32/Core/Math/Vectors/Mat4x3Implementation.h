#pragma once

#include <glm/mat4x3.hpp>

namespace Neptune
{
	template <typename T>
	class Mat4x3: public glm::tmat4x3<T>
	{

	};
}