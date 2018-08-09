#include "Camera/Controller/TempFPSCameraController.h"
#include "Graphics/Camera.h"
#include "Math/Vectors/Vec3.h"
#include <SDL2/SDL.h>

using namespace Neptune;

static void KeyDown(SDL_Event& e,Camera& camera, const float DEFAULT_STEP )
{
	const float DEFAULT_ANGLE = 1.0f;

	switch(e.key.keysym.scancode)
	{
		// Modifies position
	case SDL_SCANCODE_W:
		camera.translate(0.0f,0.0f,DEFAULT_STEP);
		break;

	case SDL_SCANCODE_S:
		camera.translate(0.0f,0.0f,-DEFAULT_STEP);
		break;

	case SDL_SCANCODE_A:
		camera.translate(-DEFAULT_STEP,0.0f,0.0f);
		break;

	case SDL_SCANCODE_D:
		camera.translate(DEFAULT_STEP,0.0f,0.0f);
		break;

	case SDL_SCANCODE_Q:
		camera.translate(0.0f,-DEFAULT_STEP,0.0f);
		break;

	case SDL_SCANCODE_E:
		camera.translate(0.0f,DEFAULT_STEP,0.0f);
		break;


		// Modifies orientation
	case SDL_SCANCODE_UP:
		camera.rotate(DEFAULT_ANGLE, Vec3(1.0f, 0.0f, 0.0f));
		break;

	case SDL_SCANCODE_DOWN:
		camera.rotate(-DEFAULT_ANGLE, Vec3(1.0f, 0.0f, 0.0f));
		break;

	case SDL_SCANCODE_LEFT:
		camera.rotate(-DEFAULT_ANGLE, Vec3(0.0f, 1.0f, 0.0f));
		break;

	case SDL_SCANCODE_RIGHT:
		camera.rotate(DEFAULT_ANGLE, Vec3(0.0f, 1.0f, 0.0f));
		break;
	}
}

static void MouseButtonDown(SDL_Event& e, Camera& camera, const float DEFAULT_STEP)
{
	switch (e.button.button)
	{
	case SDL_BUTTON_LEFT:
		//camera.translate(0.0f,-DEFAULT_STEP,0.0f);
		break;

	case SDL_BUTTON_RIGHT:
		//camera.translate(0.0f,DEFAULT_STEP,0.0f);
		break;
	}
}

// A bit buggy ()
static void MouseMotion(SDL_Event& e, Camera& camera)
{
	/*const float DEFAULT_ANGLE = 0.5f;

	if ( e.motion.xrel < 0 )
		camera.rotate(-DEFAULT_ANGLE, Vec3(0.0f, 1.0f, 0.0f));
	else if ( e.motion.xrel > 0 )
		camera.rotate(DEFAULT_ANGLE, Vec3(0.0f, 1.0f, 0.0f));

	if ( e.motion.yrel < 0 )
		camera.rotate(DEFAULT_ANGLE, Vec3(1.0f, 0.0f, 0.0f));
	else if ( e.motion.yrel > 0 )
		camera.rotate(-DEFAULT_ANGLE, Vec3(1.0f, 0.0f, 0.0f));*/
}

void TempFPSCameraController::move()
{
	if ( m_camera != nullptr )
	{
		SDL_Event e;
		float DEFAULT_STEP = 0.1f;

		while ( SDL_PollEvent(&e) > 0 )
		{
			switch (e.type)
			{
			case SDL_KEYDOWN:
				KeyDown(e, *m_camera, DEFAULT_STEP);
				break;

			case SDL_MOUSEBUTTONDOWN:
				MouseButtonDown(e, *m_camera, DEFAULT_STEP);
				break;

			case SDL_MOUSEMOTION:
				MouseMotion(e, *m_camera);
				break;
			}
		}
	}
}