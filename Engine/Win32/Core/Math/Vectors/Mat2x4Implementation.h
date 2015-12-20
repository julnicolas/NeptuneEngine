#pragma once

#include <glm/mat2x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Neptune
{
	template <typename T>
	class BaseMatrix2x4: public glm::tmat2x4<T>
	{
	public:
		BaseMatrix2x4<T>(): glm::tmat2x4<T>(1)               {}
		~BaseMatrix2x4<T>()                                  = default;
		BaseMatrix2x4<T>(const BaseMatrix2x4<T>&)            = default;
		BaseMatrix2x4<T>& operator=(const BaseMatrix2x4<T>&) = default;

		const T* getPtr() const { return glm::value_ptr(*(tmat2x4<T>*) this); }
	};


	////////////////////////////////////////////////////////////////////////////
	//
	//          I M P L E M E N TA T I O N   O F   M A T 2 x 4
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	const T* Mat2x4<T>::getPtr() const
	{
		return m_matrix.getPtr();
	}

	template <typename T>
	Mat2x4<T>& Mat2x4<T>::operator*=(const Mat2x4<T>& m)
	{
		(glm::tmat2x4<T>&) m_matrix = m_matrix * m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat2x4<T>& Mat2x4<T>::operator+=(const Mat2x4<T>& m)
	{
		(glm::tmat2x4<T>&) m_matrix = m_matrix + m.m_matrix;
		return *this;
	}

	template <typename T>
	Mat2x4<T>& Mat2x4<T>::operator-=(const Mat2x4<T>& m)
	{
		(glm::tmat2x4<T>&) m_matrix = m_matrix - m.m_matrix;
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	//
	//                 M A T 2 x 4 ' S   O P E R A T O R S
	//
	///////////////////////////////////////////////////////////////////////////

	template <typename T>
	Mat2x4<T> operator*(const Mat2x4<T>& ma,const Mat2x4<T>& mb)
	{
		using namespace glm;

		Mat2x4<T> r;
		(tmat2x4<T>&) r.getBase() = (tmat2x4<T>&) ma.getBase() * (tmat2x4<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat2x4<T> operator+(const Mat2x4<T>& ma,const Mat2x4<T>& mb)
	{
		using namespace glm;

		Mat2x4<T> r;
		(tmat2x4<T>&) r.getBase() = (tmat2x4<T>&) ma.getBase() + (tmat2x4<T>&) mb.getBase();

		return r;
	}

	template <typename T>
	Mat2x4<T> operator-(const Mat2x4<T>& ma,const Mat2x4<T>& mb)
	{
		using namespace glm;

		Mat2x4<T> r;
		(tmat2x4<T>&) r.getBase() = (tmat2x4<T>&) ma.getBase() - (tmat2x4<T>&) mb.getBase();

		return r;
	}
}