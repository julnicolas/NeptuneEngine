#pragma once

///
/// \brief Defines a position in space. 
/// It is defined as a C struct so that we are sure that size_of(Position) == 3*sizeof(float)
///

namespace Neptune
{
	extern "C"
	{
		struct Position
		{
			float m_x;
			float m_y;
			float m_z;
		};
	}

	Position operator+(const Position& c1,const Position& c2);
	Position operator+(const Position& c1,float v);

	Position operator-(const Position& c1,const Position& c2);
	Position operator-(const Position& c1,float v);

	Position operator*(const Position& c1,const Position& c2);
	Position operator*(const Position& c1,float v);

	Position operator/(const Position& c1,const Position& c2);
	Position operator/(const Position& c1,float v);
}