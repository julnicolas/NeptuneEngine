#pragma once

#include "Debug/NeptuneDebug.h"

namespace Neptune { namespace Debug { namespace Private {
	
// Declarations
void NepLog(const char* _message...);
void LogAssertErrMsg(bool _cond, const char* _condString, int _line, const char* _functionName, const char* _message...);
void GL_Assert();
void LogAssert(bool _cond);

#if defined(NEP_DEBUG) || defined(NEP_RELEASE)
	
	inline void LogAssertErrMsg(bool _cond, const char* _condString, int _line, const char* _functionName, const char* _message...)
	{
		if (!_cond)
		{
			// Assert message header
			fprintf(stderr, "## Assertion failed in function %s line %d:\n # Expression: %s == false\n # Description: ",
				_functionName, _line, _condString);
			
			// Log message and its arguments
			va_list args;
			va_start(args, _message);
			vfprintf(stderr, _message, args);
			va_end(args);
			fprintf(stderr,"\n");
		}
	}

	inline void GL_Assert()
	{
		auto error = glGetError();
		if (error)
		{
			NEP_LOG("ERROR: GLenum for error == %x", error);
			NEP_ASSERT(error == GL_NO_ERROR);
		}
	}
#endif

#ifdef NEP_RELEASE
	inline void LogAssert(bool _cond) { if (!_cond) {NEP_LOG("Assertion failed: file: %s, line: %d", __FILE__, __LINE__);} }
#endif

#ifndef NEP_FINAL
	inline void NepLog(const char* _message...)
	{
		// Log message and its arguments
		va_list args;
		va_start(args, _message);
		vfprintf(stderr, _message, args);
		va_end(args);
		fprintf(stderr,"\n");
	}
#endif

}}}

