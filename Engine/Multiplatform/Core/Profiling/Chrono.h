#pragma once

#include <chrono>

/// \file This file defines chronometers to profile execution time. Profiling tests are executed for every configurations except Final.


//////////
//
// Scope-restricted chronometers - every function calls must be in the same scope. 
//
/////////

#ifndef NEP_FINAL
	/// Init the profiling system
	#define NEP_PROFILING_CHRONO_INIT std::chrono::high_resolution_clock::time_point __nep__profiling__chrono__start

	/// \macro		Starts a chronometer. Can be stopped by calling NEP_PROFILING_CHRONO_STOP. 
	/// \warning	Calls to NEP_PROFILING_CHRONO_START cannot be nested. NEP_PROFILING_CHRONO_STOP must be called before a new call to
	///				NEP_PROFILING_CHRONO_START can be done in the same scope.
	#define NEP_PROFILING_CHRONO_START	__nep__profiling__chrono__start = Neptune::Chrono::Start()

	/// \macro Stops the chronometer started by NEP_PROFILING_CHRONO_START.
	#define NEP_PROFILING_CHRONO_STOP	Neptune::Chrono::Stop(__nep__profiling__chrono__start)
#else
	#define NEP_PROFILING_CHRONO_INIT	
	#define NEP_PROFILING_CHRONO_START
	#define NEP_PROFILING_CHRONO_STOP	0.0
#endif


//////////
//
// DECLARATIONS - use the macros above to do the profiling tests (the calls below won't be disabled in Final config).
//
//////////

namespace Neptune
{
	namespace Chrono
	{
		std::chrono::high_resolution_clock::time_point	Start();							/// Starts chronometer. Returns the start time.
		double	Stop(const std::chrono::high_resolution_clock::time_point& _startTime);		/// Stops chronometer. Returns the duration between _startTime and the call to Stop() in milli seconds.
	}
}