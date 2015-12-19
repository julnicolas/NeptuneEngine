#pragma once

namespace Neptune
{
	// Matrix with 4 rows and 4 columns
	template <typename T> class Mat4x4;
	typedef Mat4x4<float> Mat4;
}

// Platform-specific Implementation
#include "Math/Vectors/Mat4Implementation.h" 