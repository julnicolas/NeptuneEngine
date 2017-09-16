#pragma once

#include "System/Type/Integers.h"

namespace Neptune
{
	inline u64 Fnv1a64(const u8* data, u64 size)
	{
		const u64 FNV_OFFSET_BASIS = 14695981039346656037ull;
		const u64 FNV_PRIME        = 1099511628211ull;
		u64 hash                   = FNV_OFFSET_BASIS;

		for(u64 i = 0; i < size; i++)
		{
			hash ^= data[i];
			hash *= FNV_PRIME;
		}

		return hash;
	}

	inline u32 Fnv1a32(const u8* data, u64 size)
	{
		const u32 FNV_OFFSET_BASIS = 0x811C9DC5;
		const u32 FNV_PRIME        = 0x01000193;
		u32 hash                   = FNV_OFFSET_BASIS;

		for(u64 i = 0; i < size; i++)
		{
			hash ^= data[i];
			hash *= FNV_PRIME;
		}

		return hash;
	}
}