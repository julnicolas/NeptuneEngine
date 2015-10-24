#include "WinApp.h"
#include "SDL2/SDL.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

WinApp::WinApp()
{
	// Create a SDL event to pump the queue
	m_inputEvent = new SDL_Event;
}

WinApp::~WinApp()
{
	delete m_inputEvent;
}

bool WinApp::init()
{
	// Init input-fetching system
	NEP_ASSERT( SDL_Init(0) >= 0 );
	SDL_InitSubSystem( SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER );

	return true;
}

void WinApp::terminate()
{
	SDL_QuitSubSystem( SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER );
}

bool WinApp::fetchAndCopyFirstInputEvent()
{
	return SDL_PollEvent(m_inputEvent) == 1;
}