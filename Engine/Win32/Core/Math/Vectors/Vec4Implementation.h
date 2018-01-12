#pragma once

#include <glm/vec4.hpp>

namespace Neptune
{
	template <typename T>
	class BaseVec4_t: public glm::tvec4<T>
	{
	public:
		BaseVec4_t<T>(): glm::tvec4<T>(0)              {}
		~BaseVec4_t<T>()                               = default;
		BaseVec4_t<T>(const BaseVec4_t<T>&)            = default;
		BaseVec4_t<T>& operator=(const BaseVec4_t<T>&) = default;

		BaseVec4_t<T>(T v1,T v2,T v3, T v4): glm::tvec4<T>(v1,v2,v3,v4) {}

		const T* getPtr() const {
			return glm::value_ptr(*static_cast<const glm::tvec4<T>*>(this));
		}
	};

	template <typename T>
	Vec4_t<T>::Vec4_t(T v1,T v2,T v3, T v4): m_vec(v1,v2,v3,v4) {}

	template <typename T> T& Vec4_t<T>::x() { return m_vec.x; }
	template <typename T> T& Vec4_t<T>::y() { return m_vec.y; }
	template <typename T> T& Vec4_t<T>::z() { return m_vec.z; }
	template <typename T> T& Vec4_t<T>::w() { return m_vec.w; }

	template <typename T>
	const T* Vec4_t<T>::getPtr() const
	{
		return m_vec.getPtr();
	}
}