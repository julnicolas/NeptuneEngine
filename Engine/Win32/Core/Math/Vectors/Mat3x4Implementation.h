#pragma once

#include <glm/mat3x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Neptune
{
	template <typename T>
	class BaseMatrix3x4: public glm::tmat3x4<T>
	{
	public:
		BaseMatrix3x4<T>(): glm::tmat3x4<T>(1)               {}
		~BaseMatrix3x4<T>()                                  = default;
		BaseMatrix3x4<T>(const BaseMatrix3x4<T>&)            = default;
		BaseMatrix3x4<T>& operator=(const BaseMatrix3x4<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*(tmat3x4<T>*) this); }
	};


	////////////////////////////////////////////////////////////////////////////
	//
	//          I M P L E M E N TA T I O N   O F   M A T 3 x 4
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	const T* Mat3x4<T>::getPtr() const
	{
		return m_matrix.getPtr();
	}

	template <typename T>
	Mat3x4<T>& Mat3x4<T>::operator*=(const Mat3x4<T>& m)
	{
		(glm::tmat3x4<T>&) m_matrix = m_matrix * m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat3x4<T>& Mat3x4<T>::operator+=(const Mat3x4<T>& m)
	{
		(glm::tmat3x4<T>&) m_matrix = m_matrix + m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat3x4<T>& Mat3x4<T>::operator-=(const Mat3x4<T>& m)
	{
		(glm::tmat3x4<T>&) m_matrix = m_matrix - m.m_matrix;
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	//
	//                 M A T 3 x 4 ' S   O P E R A T O R S
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	Mat3x4<T> operator*(const Mat3x4<T>& ma,const Mat3x4<T>& mb)
	{
		using namespace glm;

		Mat3x4<T> r;
		(tmat3x4<T>&) r.getBase() = (tmat3x4<T>&) ma.getBase() * (tmat3x4<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat3x4<T> operator+(const Mat3x4<T>& ma,const Mat3x4<T>& mb)
	{
		using namespace glm;

		Mat3x4<T> r;
		(tmat3x4<T>&) r.getBase() = (tmat3x4<T>&) ma.getBase() + (tmat3x4<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat3x4<T> operator-(const Mat3x4<T>& ma,const Mat3x4<T>& mb)
	{
		using namespace glm;

		Mat3x4<T> r;
		(tmat3x4<T>&) r.getBase() = (tmat3x4<T>&) ma.getBase() - (tmat3x4<T>&) mb.getBase();

		return r;
	}
}