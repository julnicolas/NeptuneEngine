#pragma once

#include <glm/vec3.hpp>

namespace Neptune
{
	template <typename T>
	class BaseVec3_t: public glm::tvec3<T>
	{
	public:
		BaseVec3_t<T>(): glm::tvec3<T>(0)              {}
		~BaseVec3_t<T>()                               = default;
		BaseVec3_t<T>(const BaseVec3_t<T>&)            = default;
		BaseVec3_t<T>& operator=(const BaseVec3_t<T>&) = default;

		BaseVec3_t<T>(T v1, T v2, T v3): glm::tvec3<T>(v1, v2, v3) {}
		const T* getPtr() const {
			return glm::value_ptr(*static_cast<const glm::tvec3<T>*>(this));
		}
	};

	template <typename T>
	Vec3_t<T>::Vec3_t(T v1, T v2, T v3): m_vec(v1,v2,v3) {}

	template <typename T> T& Vec3_t<T>::x() { return m_vec.x; }
	template <typename T> T& Vec3_t<T>::y() { return m_vec.y; }
	template <typename T> T& Vec3_t<T>::z() { return m_vec.z; }

	template <typename T>
	const T* Vec3_t<T>::getPtr() const
	{
		return m_vec.getPtr();
	}
}