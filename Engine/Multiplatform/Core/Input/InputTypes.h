#pragma once
#include "System/Type/Integers.h"
#include "Input/PlatformSpecificInputTypes.h"


namespace Neptune
{
	enum class InputType : u8
	{
		ANY,				/// Refers to all events
		KEYBOARD_PUSH,		/// When a keyboard key is pressed
		KEYBOARD_RELEASE,	/// When a keyboard key is released
		WINDOW,				/// OS-window-related-input such as close, minimise, maximise... 
	};
}