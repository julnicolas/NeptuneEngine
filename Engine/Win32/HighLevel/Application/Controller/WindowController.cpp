#pragma once
#include "Application/SimpleApp.h"
#include "Application/Controller/WindowController.h"
#include "Debug/NeptuneDebug.h"
#include <SDL2/SDL.h>


using namespace Neptune;

void WindowController::Close(const Input& _input, SimpleApp* _app)
{
	NEP_ASSERT_ERR_MSG(_app != nullptr, "Invalid pointer");
	NEP_ASSERT_ERR_MSG(_input.type == SDL_WINDOWEVENT, "Invalid input type, input received is not a window input (_input.type == 0x%x)", _input.type);

	if (_input.window.event == SDL_WINDOWEVENT_CLOSE)
		_app->stopUpdate();
}