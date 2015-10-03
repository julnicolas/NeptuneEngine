#pragma once

#include <glm/mat4x4.hpp>

namespace Neptune
{
	template <typename T>
	class Mat4: public glm::tmat4x4<T>
	{

	};
}