#include "Camera/Controller/TempFPSCameraController.h"
#include "Graphics/Camera.h"
#include "Math/Math.h"
#include "Math/Vectors/Vec3.h"
#include "Math/Vectors/MatrixTransform.h"
#include <SDL2/SDL.h>

using namespace Neptune;

const float DEFAULT_STEP = 0.1f;

TempFPSCameraController::TempFPSCameraController() :
m_camera(nullptr),
m_stepDistance(DEFAULT_STEP)
{

}

TempFPSCameraController::TempFPSCameraController(Camera* camera) :
m_camera(camera),
m_stepDistance(DEFAULT_STEP)
{

}

static void KeyDown(SDL_Event& e,Camera& camera, float& step_distance )
{
	const float DEFAULT_ANGLE = 1.0f;

	const Mat4& orientation_matrix = Transpose(camera.getOrientation());
	Vec4 step(0.0f, 0.0f, 0.0f, 0.0f);


	switch(e.key.keysym.scancode)
	{
		// Change acceleration
	case SDL_SCANCODE_Z:
		step_distance += 0.1f;
		break;

	case SDL_SCANCODE_X:
		step_distance = Max(0.1f, step_distance - 0.1f);
		break;
		
		// Change position
	case SDL_SCANCODE_W:
		step.z() = step_distance;
		step = orientation_matrix * step;
		camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_S:
		step.z() = -step_distance;
		step = orientation_matrix * step;
		camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_A:
		step.x() = -step_distance;
		step = orientation_matrix * step;
		camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_D:
		step.x() = step_distance;
		step = orientation_matrix * step;
		camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_Q:
		step.y() = -step_distance;
		step = orientation_matrix * step;
		camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_E:
		step.y() = step_distance;
		step = orientation_matrix * step;
		camera.translate(step.x(), step.y(), step.z());
		break;


		// Change orientation
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

		while ( SDL_PollEvent(&e) > 0 )
		{
			switch (e.type)
			{
			case SDL_KEYDOWN:
				KeyDown(e, *m_camera, m_stepDistance);
				break;

			case SDL_MOUSEBUTTONDOWN:
				MouseButtonDown(e, *m_camera, m_stepDistance);
				break;

			case SDL_MOUSEMOTION:
				MouseMotion(e, *m_camera);
				break;
			}
		}
	}
}