#pragma once

#include <glm/mat3x4.hpp>

namespace Neptune
{
	template <typename T>
	class Mat3x4: public glm::tmat3x4<T>
	{

	};
}