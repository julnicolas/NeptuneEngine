#pragma once

#include "System/Type/Integers.h"
#include <string>

namespace Neptune
{
	namespace PathHelpers
	{
		/// Returns the position of the first character of the extension. MAX_U32 if no extension is found.
		u32 GetFileExtension(const char* _path);

		/// \brief Returns the extension if found, empty string otherwise.
		/// \note For A.png the function returns .png
		std::string GetFileExtension(const std::string& _path);
	}
}