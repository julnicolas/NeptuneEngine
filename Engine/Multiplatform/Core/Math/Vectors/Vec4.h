#pragma once

namespace Neptune
{
	// Matrix with 2 rows and 2 columns
	template <typename T> class Vec4_t;
	typedef Vec4_t<float> Vec4;
}

// Platform-specific Implementation
#include "Math/Vectors/Vec4Implementation.h" 