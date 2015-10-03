#pragma once

#include <glm/mat3x3.hpp>

namespace Neptune
{
	template <typename T>
	class Mat3: public glm::tmat3x3<T>
	{

	};
}