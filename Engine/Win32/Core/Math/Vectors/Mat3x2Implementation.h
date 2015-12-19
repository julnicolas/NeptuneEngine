#pragma once

#include <glm/mat3x2.hpp>

namespace Neptune
{
	template <typename T>
	class Mat3x2: public glm::tmat3x2<T>
	{
		Mat3x2<T>()                            = default;
		~Mat3x2<T>()                           = default;
		Mat3x2<T>(const Mat3x2<T>&)            = default;
		Mat3x2<T>& operator=(const Mat3x2<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}