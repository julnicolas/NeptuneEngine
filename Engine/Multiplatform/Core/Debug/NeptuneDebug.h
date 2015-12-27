#pragma once

#include <cstdio>
#include <cassert>

#ifndef NEP_FINAL
	// Variadic macros have become standard since c++11
	#define NEP_LOG(...) std::fprintf(stderr, __VA_ARGS__)
#else
	// The statement between NEP_LOG's brackets won't be executed 
	#define NEP_LOG(err) ((void) 0)
#endif

#ifdef NEP_DEBUG
	#define NEP_ASSERT	assert
#else
	#define NEP_ASSERT
#endif
