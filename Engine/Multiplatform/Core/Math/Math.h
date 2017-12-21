#pragma once

namespace Neptune
{
	template <typename T>
	inline T Min(const T& t1, const T& t2)	{ return (t1 < t2) ? t1 : t2; }

	template <typename T>
	inline T Max(const T& t1, const T& t2)	{ return (t1 > t2) ? t1 : t2; }
}