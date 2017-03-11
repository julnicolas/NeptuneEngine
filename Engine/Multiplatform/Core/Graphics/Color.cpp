#include "Graphics/Color.h"
#include "Math/Math.h"

using namespace Neptune;

static float NormalizeColor(float c)
{
	return Max( Min(c, 1.0f), 0.0f); // returns a value between 0.0f and 1.0f. 0.0f if < 0.0f, 1.0f if > 1.0f
}

Color operator+(const Color& c1,const Color& c2)
{
	Color res;
	res.r = NormalizeColor( c1.r + c2.r );
	res.g = NormalizeColor( c1.g + c2.g );
	res.b = NormalizeColor( c1.b + c2.b );

	return res;
}

Color operator+(const Color& c1,float v)
{
	Color res;
	res.r = NormalizeColor( c1.r + v );
	res.g = NormalizeColor( c1.g + v );
	res.b = NormalizeColor( c1.b + v );

	return res;
}

Color operator-(const Color& c1,const Color& c2)
{
	Color res;
	res.r = NormalizeColor( c1.r - c2.r );
	res.g = NormalizeColor( c1.g - c2.g );
	res.b = NormalizeColor( c1.b - c2.b );

	return res;
}

Color operator-(const Color& c1,float v)
{
	Color res;
	res.r = NormalizeColor( c1.r - v );
	res.g = NormalizeColor( c1.g - v );
	res.b = NormalizeColor( c1.b - v );

	return res;
}

Color operator*(const Color& c1,const Color& c2)
{
	Color res;
	res.r = NormalizeColor( c1.r * c2.r );
	res.g = NormalizeColor( c1.g * c2.g );
	res.b = NormalizeColor( c1.b * c2.b );

	return res;
}

Color operator*(const Color& c1,float v)
{
	Color res;
	res.r = NormalizeColor( c1.r * v );
	res.g = NormalizeColor( c1.g * v );
	res.b = NormalizeColor( c1.b * v );

	return res;
}

Color operator/(const Color& c1,const Color& c2)
{
	Color res;
	res.r = NormalizeColor( c1.r / c2.r );
	res.g = NormalizeColor( c1.g / c2.g );
	res.b = NormalizeColor( c1.b / c2.b );

	return res;
}

Color operator/(const Color& c1,float v)
{
	Color res;
	res.r = NormalizeColor( c1.r / v );
	res.g = NormalizeColor( c1.g / v );
	res.b = NormalizeColor( c1.b / v );

	return res;
}