#include "Input/EventSystemInterface.h"
#include "Debug/NeptuneDebug.h"
#include <SDL2/SDL.h>

using namespace Neptune;

bool  EventSystemInterface::StartUp()
{
	if ( SDL_InitSubSystem(SDL_INIT_EVENTS) < 0 )
	{
		NEP_LOG("Error: Couldn't initialize event system. \nThe following error has been raised:\n%s", SDL_GetError());
		return false;
	}

	return true;
}

void EventSystemInterface::ShutDown()
{
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
}