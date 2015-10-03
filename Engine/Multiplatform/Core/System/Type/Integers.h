#pragma  once

// The engine's types comply with the following standards: ILP32, LLP64 (Windows), LP64 (OSX, Linux)
// They don't comply with the C++ and LP32 standards.

namespace Neptune
{
	// Signed integers
	typedef signed long long int s64;
	typedef signed int           s32;
	typedef signed short         s16;
	typedef signed char          s8;
	
	// Unsigned integers
	typedef unsigned long long int u64;
	typedef unsigned int           u32;
	typedef unsigned short         u16;
	typedef unsigned char          u8;
}