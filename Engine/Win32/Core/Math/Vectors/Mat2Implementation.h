#pragma once

#include <glm/mat2x2.hpp>

namespace Neptune
{
	template <typename T>
	class Mat2: public glm::tmat2x2<T>
	{
		Mat2<T>()                          = default;
		~Mat2<T>()                         = default;
		Mat2<T>(const Mat2<T>&)            = default;
		Mat2<T>& operator=(const Mat2<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}