#pragma once

#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstdarg>
#include "Graphics/IncludeOpenGL.h"

/////////////////////////////////////////////////////////////////////////////
//
//								L O G S 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef NEP_FINAL
	// Variadic macros have become standard since c++11
	#define NEP_LOG(...) Neptune::Debug::Private::NepLog(__VA_ARGS__)
#else
	// The statement between NEP_LOG's brackets won't be executed 
	#define NEP_LOG(...) ((void) 0)
#endif


/////////////////////////////////////////////////////////////////////////////
//
//							A S S E R T I O N S
//
/////////////////////////////////////////////////////////////////////////////


#define NEP_STATIC_ASSERT(expression, message) static_assert(expression, message)

#ifdef NEP_DEBUG
	// Standard assert
	#define NEP_ASSERT	assert
	
	// Standard assert with custom error message (printf format)
#define NEP_ASSERT_ERR_MSG(_cond, ...) {Neptune::Debug::Private::LogAssertErrMsg(_cond, #_cond, __LINE__, __FUNCTION__, __VA_ARGS__); if (_cond == false){ abort();}}
	
	// Graphics assert - use to check whether a call to the graphics API generated an error
	#define NEP_GRAPHICS_ASSERT() Neptune::Debug::Private::GL_Assert()
#endif

#ifdef NEP_RELEASE
	#define NEP_ASSERT(_cond) Neptune::Debug::Private::LogAssert(_cond)
	#define NEP_ASSERT_ERR_MSG(_cond, ...) Neptune::Debug::Private::LogAssertErrMsg(_cond, #_cond, __LINE__, __FUNCTION__, __VA_ARGS__)
	#define NEP_GRAPHICS_ASSERT() Neptune::Debug::Private::GL_Assert()
#endif

#ifdef NEP_FINAL
	// The statement between the brackets won't be executed 
	#define NEP_ASSERT(_cond) ((void) 0)
	#define NEP_ASSERT_ERR_MSG(_cond, ...) ((void) 0)
	#define NEP_GRAPHICS_ASSERT() ((void) 0)
#endif

#include "Debug/NeptuneDebugInlineImplementations.h"