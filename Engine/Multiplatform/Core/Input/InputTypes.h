#pragma once
#include "System/Type/Integers.h"


namespace Neptune
{
	typedef void* Input;

	enum class InputType : u8
	{
		ANY,				/// Refers to all events
		KEYBOARD_PUSH,		/// When a keyboard key is pressed
		KEYBOARD_RELEASE,	/// When a keyboard key is released
	};
}