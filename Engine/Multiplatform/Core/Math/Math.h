#pragma once

#include "System/NeptuneTypes.h"

namespace Neptune
{
	inline u64    Min(u64 x, u64 y)          { return (x < y) ? x : y; }
	inline s64    Min(s64 x, s64 y)          { return (x < y) ? x : y; }
	inline u32    Min(u32 x, u32 y)          { return (x < y) ? x : y; }
	inline s32    Min(s32 x, s32 y)          { return (x < y) ? x : y; }
	inline u8     Min(u8 x, u8 y)            { return (x < y) ? x : y; }
	inline s8     Min(s8 x, s8 y)            { return (x < y) ? x : y; }
	inline float  Min(float x, float y)      { return (x < y) ? x : y; }
	inline double Min(double x, double y)    { return (x < y) ? x : y; }

	inline u64    Max(u64 x, u64 y)          { return (x > y) ? x : y; }
	inline s64    Max(s64 x, s64 y)          { return (x > y) ? x : y; }
	inline u32    Max(u32 x, u32 y)          { return (x > y) ? x : y; }
	inline s32    Max(s32 x, s32 y)          { return (x > y) ? x : y; }
	inline u8     Max(u8 x, u8 y)            { return (x > y) ? x : y; }
	inline s8     Max(s8 x, s8 y)            { return (x > y) ? x : y; }
	inline float  Max(float x, float y)      { return (x > y) ? x : y; }
	inline double Max(double x, double y)    { return (x > y) ? x : y; }
}