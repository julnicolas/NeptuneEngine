#pragma once

#include <cstdio>
#include <cassert>
#include "Graphics/IncludeOpenGL.h"


/////////////////////////////////////////////////////////////////////////////
//
//								L O G S 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef NEP_FINAL
	// Variadic macros have become standard since c++11
	#define NEP_LOG(...) std::fprintf(stderr, __VA_ARGS__) // TODO: Change stderr by something more useful
#else
	// The statement between NEP_LOG's brackets won't be executed 
	#define NEP_LOG(err) ((void) 0)
#endif


/////////////////////////////////////////////////////////////////////////////
//
//							A S S E R T I O N S
//
/////////////////////////////////////////////////////////////////////////////


#define NEP_STATIC_ASSERT(expression, message) static_assert(expression, message)

#ifdef NEP_DEBUG
	#define NEP_ASSERT	assert
#endif

#ifdef NEP_RELEASE
namespace Neptune
{
	namespace Debug
	{
		namespace Private
		{
			inline void LogAssert(bool _cond) { if (!_cond) NEP_LOG("Assertion failed: file: %s, line: %d", __FILE__, __LINE__); }
		}
	}
}
	#define NEP_ASSERT(_cond) Neptune::Debug::Private::LogAssert(_cond)
#endif

#ifdef NEP_FINAL
	// The statement between the brackets won't be executed 
	#define NEP_ASSERT(_cond) ((void) 0)
#endif


// Graphics assert - use to check whether a call to the graphics API generated an error

namespace Neptune
{
	namespace Debug
	{
		namespace Private
		{
			void GL_ASSERT();
		}
	}
}

/// Use to check after a graphics API call
#define NEP_GRAPHICS_ASSERT Neptune::Debug::Private::GL_ASSERT

#if defined(NEP_DEBUG) || defined(NEP_RELEASE)
	inline void Neptune::Debug::Private::GL_ASSERT()
	{
		auto error = glGetError();
		if (error)
		{
			NEP_LOG("ERROR: GLenum for error == %x", error);
			NEP_ASSERT(error == GL_NO_ERROR);
		}
	}
#endif

#ifdef NEP_FINAL
	inline void Neptune::Debug::Private::GL_ASSERT(){}
#endif