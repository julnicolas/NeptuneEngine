#pragma once
#include <SDL2/SDL.h>

/// Platform specific (Win32) input types must be defined in this file
/// It is included by Multiplatform/Core/Input/InputTypes.h

/// Windows platform uses the SDL to handle inputs
namespace Neptune 
{
	typedef SDL_Event Input;
}