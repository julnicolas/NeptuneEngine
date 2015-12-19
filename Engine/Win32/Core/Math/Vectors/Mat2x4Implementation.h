#pragma once

#include <glm/mat2x4.hpp>

namespace Neptune
{
	template <typename T>
	class Mat2x4: public glm::tmat2x4<T>
	{
		Mat2x4<T>()                            = default;
		~Mat2x4<T>()                           = default;
		Mat2x4<T>(const Mat2x4<T>&)            = default;
		Mat2x4<T>& operator=(const Mat2x4<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}