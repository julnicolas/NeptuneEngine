#pragma once

namespace Neptune
{
	// Matrix with 2 rows and 2 columns
	template <typename T> class Vec2_t;
	typedef Vec2_t<float> Vec2;
}

// Platform-specific Implementation
#include "Math/Vectors/Vec2Implementation.h" 