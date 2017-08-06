#include "Position.h"

using namespace Neptune;

Position operator+(const Position& c1,const Position& c2)
{
	Position res;
	res.m_x = c1.m_x + c2.m_x;
	res.m_y = c1.m_y + c2.m_y;
	res.m_z = c1.m_z + c2.m_z;

	return res;
}

Position operator+(const Position& c1,float v)
{
	Position res;
	res.m_x = c1.m_x + v;
	res.m_y = c1.m_y + v;
	res.m_z = c1.m_z + v;

	return res;
}

Position operator-(const Position& c1,const Position& c2)
{
	Position res;
	res.m_x = c1.m_x - c2.m_x;
	res.m_y = c1.m_y - c2.m_y;
	res.m_z = c1.m_z - c2.m_z;

	return res;
}

Position operator-(const Position& c1,float v)
{
	Position res;
	res.m_x = c1.m_x - v;
	res.m_y = c1.m_y - v;
	res.m_z = c1.m_z - v;

	return res;
}

Position operator*(const Position& c1,const Position& c2)
{
	Position res;
	res.m_x = c1.m_x * c2.m_x;
	res.m_y = c1.m_y * c2.m_y;
	res.m_z = c1.m_z * c2.m_z;

	return res;
}

Position operator*(const Position& c1,float v)
{
	Position res;
	res.m_x = c1.m_x * v;
	res.m_y = c1.m_y * v;
	res.m_z = c1.m_z * v;

	return res;
}

Position operator/(const Position& c1,const Position& c2)
{
	Position res;
	res.m_x = c1.m_x / c2.m_x;
	res.m_y = c1.m_y / c2.m_y;
	res.m_z = c1.m_z / c2.m_z;

	return res;
}

Position operator/(const Position& c1,float v)
{
	Position res;
	res.m_x = c1.m_x / v;
	res.m_y = c1.m_y / v;
	res.m_z = c1.m_z / v;

	return res;
}
