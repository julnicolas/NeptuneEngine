#pragma once

#include <glm/mat3x3.hpp>

namespace Neptune
{
	template <typename T>
	class Mat3: public glm::tmat3x3<T>
	{
		Mat3<T>()                          = default;
		~Mat3<T>()                         = default;
		Mat3<T>(const Mat3<T>&)            = default;
		Mat3<T>& operator=(const Mat3<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}