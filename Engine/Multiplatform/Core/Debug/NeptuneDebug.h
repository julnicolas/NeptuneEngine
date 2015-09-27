#ifndef NEPTUNE_DEBUG_H_
#define NEPTUNE_DEBUG_H_

#include <iostream>
#include <assert.h>


// Declarations

void nepLog(const char* err);


// Definitions

inline void nepLog(const char* err)
{
	using namespace std;

	cerr << err << endl;
}


#ifdef NEP_DEBUG
	#define NEP_LOG(err) nepLog(err)
	#define NEP_ASSERT	assert
#else
	#define NEP_LOG(err)
	#define NEP_ASSERT
#endif


#endif