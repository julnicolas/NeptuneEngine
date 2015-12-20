#pragma once

#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Neptune
{
	template <typename T>
	class BaseMatrix3x3: public glm::tmat3x3<T>
	{
	public:
		BaseMatrix3x3<T>(): glm::tmat3x3<T>(1)               {}
		~BaseMatrix3x3<T>()                                  = default;
		BaseMatrix3x3<T>(const BaseMatrix3x3<T>&)            = default;
		BaseMatrix3x3<T>& operator=(const BaseMatrix3x3<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*(tmat3x3<T>*) this); }
	};


	////////////////////////////////////////////////////////////////////////////
	//
	//          I M P L E M E N TA T I O N   O F   M A T 3 x 3
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	const T* Mat3x3<T>::getPtr() const
	{
		return m_matrix.getPtr();
	}

	template <typename T>
	Mat3x3<T>& Mat3x3<T>::operator*=(const Mat3x3<T>& m)
	{
		(glm::tmat3x3<T>&) m_matrix = m_matrix * m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat3x3<T>& Mat3x3<T>::operator+=(const Mat3x3<T>& m)
	{
		(glm::tmat3x3<T>&) m_matrix = m_matrix + m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat3x3<T>& Mat3x3<T>::operator-=(const Mat3x3<T>& m)
	{
		(glm::tmat3x3<T>&) m_matrix = m_matrix - m.m_matrix;
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	//
	//                 M A T 3 x 3 ' S   O P E R A T O R S
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	Mat3x3<T> operator*(const Mat3x3<T>& ma,const Mat3x3<T>& mb)
	{
		using namespace glm;

		Mat3x3<T> r;
		(tmat3x3<T>&) r.getBase() = (tmat3x3<T>&) ma.getBase() * (tmat3x3<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat3x3<T> operator+(const Mat3x3<T>& ma,const Mat3x3<T>& mb)
	{
		using namespace glm;

		Mat3x3<T> r;
		(tmat3x3<T>&) r.getBase() = (tmat3x3<T>&) ma.getBase() + (tmat3x3<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat3x3<T> operator-(const Mat3x3<T>& ma,const Mat3x3<T>& mb)
	{
		using namespace glm;

		Mat3x3<T> r;
		(tmat3x3<T>&) r.getBase() = (tmat3x3<T>&) ma.getBase() - (tmat3x3<T>&) mb.getBase();

		return r;
	}
}