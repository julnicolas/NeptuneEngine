#pragma once

namespace Neptune
{
	// Matrix with 2 rows and 2 columns
	template <typename T> class Mat2x2;
	typedef Mat2x2<float> Mat2;
}

// Platform-specific Implementation
#include "Math/Vectors/Mat2Implementation.h" 