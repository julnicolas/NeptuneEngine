#pragma once

#include <glm/mat3x4.hpp>

namespace Neptune
{
	template <typename T>
	class Mat3x4: public glm::tmat3x4<T>
	{
		Mat3x4<T>()                            = default;
		~Mat3x4<T>()                           = default;
		Mat3x4<T>(const Mat3x4<T>&)            = default;
		Mat3x4<T>& operator=(const Mat3x4<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}