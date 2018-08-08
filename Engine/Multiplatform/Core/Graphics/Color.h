#pragma once

// Defines color types and associated conventions


namespace Neptune
{
	extern "C"
	{
		// A C struct is used so that we are sure it has the same layout as a float[4]
		// A pointer to this struct can therefore be passed safely to any rendering functions.
		struct Color
		{
			float r; /// Value between 0.0f and 1.0f
			float g; /// Value between 0.0f and 1.0f
			float b; /// Value between 0.0f and 1.0f
			float a; /// Value between 0.0f and 1.0f where 0 is fully opaque
		};
	}

	Color operator+(const Color& c1,const Color& c2);
	Color operator+(const Color& c1,float v);

	Color operator-(const Color& c1,const Color& c2);
	Color operator-(const Color& c1,float v);

	Color operator*(const Color& c1,const Color& c2);
	Color operator*(const Color& c1, float v);

	Color operator/(const Color& c1,const Color& c2);
	Color operator/(const Color& c1,float v);
}

