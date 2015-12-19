#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Neptune
{
	template <typename T>
	class Mat4x4: public glm::tmat4x4<T>
	{
	public:
		Mat4x4<T>(): glm::tmat4x4<T>(1)      {}                          
		~Mat4x4<T>()                         = default;
		Mat4x4<T>(const Mat4x4<T>&)            = default;
		Mat4x4<T>& operator=(const Mat4x4<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(* (tmat4x4<T>*) this); }
	};
}