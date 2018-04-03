#pragma once

#include "System/Type/Integers.h"

/// \file This file contains standard error codes than can be used in client applications (i.e games...).
/// Codes are available per type.
/// \warning Before using these codes, make sure values returned by user functions don't collide with the codes.
/// \note Current definitions must be replaced by c++11's constexpr definitions.

// U8 error codes
#define NEP_STD_U8_ERROR_CODE_0 static_cast<Neptune::u8>(~0)
#define NEP_STD_U8_ERROR_CODE_1 static_cast<Neptune::u8>(~0-1)
#define NEP_STD_U8_ERROR_CODE_2 static_cast<Neptune::u8>(~0-2)