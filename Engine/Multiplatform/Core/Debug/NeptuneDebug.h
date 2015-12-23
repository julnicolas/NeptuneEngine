#ifndef NEPTUNE_DEBUG_H_
#define NEPTUNE_DEBUG_H_

#include <cstdio>
#include <cassert>

#ifdef NEP_DEBUG
	// Variadic macros became standard since c++11
	#define NEP_LOG(...) std::fprintf(stderr, __VA_ARGS__)
	#define NEP_ASSERT	assert
#else
	#define NEP_LOG(err)
	#define NEP_ASSERT
#endif


#endif