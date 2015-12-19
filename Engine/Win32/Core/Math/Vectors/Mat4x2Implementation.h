#pragma once

#include <glm/mat4x2.hpp>

namespace Neptune
{
	template <typename T>
	class Mat4x2: public glm::tmat4x2<T>
	{
		Mat4x2<T>()                            = default;
		~Mat4x2<T>()                           = default;
		Mat4x2<T>(const Mat4x2<T>&)            = default;
		Mat4x2<T>& operator=(const Mat4x2<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*this); }
	};
}