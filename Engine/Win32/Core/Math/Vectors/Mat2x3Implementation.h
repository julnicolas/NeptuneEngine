#pragma once

#include <glm/mat2x3.hpp>

namespace Neptune
{
	template <typename T>
	class Mat2x3: public glm::tmat2x3<T>
	{
		Mat2x3<T>()                          = default;
		~Mat2x3<T>()                         = default;
		Mat2x3<T>(const Mat2x3<T>&)            = default;
		Mat2x3<T>& operator=(const Mat2x3<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}