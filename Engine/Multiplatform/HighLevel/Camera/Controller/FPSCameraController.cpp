#include "Camera/Controller/FPSCameraController.h"
#include "Debug/NeptuneDebug.h"
#include "Graphics/Camera.h"
#include <SDL2/SDL.h>

using namespace Neptune;

FPSCameraController::FPSCameraController()
{

}

FPSCameraController::FPSCameraController(Camera* camera)
{

}

void FPSCameraController::process(InputEvent* e)
{
	NEP_ASSERT( e != NULL );
	switch( e->type )
	{
	case SDL_KEYDOWN:
		pressedKeyEvent(e);
		break;

	case SDL_KEYUP:
		releasedKeyEvent(e);
		break;
	}
}

void FPSCameraController::pressedKeyEvent(InputEvent* e)
{
	if ( e->key.state == SDL_PRESSED || e->key.repeat )
	{
		switch ( e->key.keysym.sym )
		{
		case SDLK_UP:
			m_camera->translate( glm::vec3(0.0f, 1.0f, 0.0f) );
			break;

		case SDLK_DOWN:
			m_camera->translate( glm::vec3(0.0f, -1.0f, 0.0f) );
			break;

		case SDLK_LEFT:
			m_camera->translate( glm::vec3(-1.0f, 0.0f, 0.0f) );
			break;

		case SDLK_RIGHT:
			m_camera->translate( glm::vec3(1.0f, 1.0f, 0.0f) );
			break;
		}
	}
	/*else
		if ( e->key.state == SDL_RELEASED )
		{
			switch ( e->key.keysym.sym )
			{
			case SDLK_UP:

				break;

			case SDLK_DOWN:

				break;

			case SDLK_LEFT:

				break;

			case SDLK_RIGHT:

				break;
			}
		}*/
}

void FPSCameraController::releasedKeyEvent(InputEvent* e)
{

}