#include "Camera/Controller/FPSController.h"
#include "Input/InputTypes.h"
#include "Graphics/Camera.h"
#include "Math/Vectors/Mat4x4.h"
#include "Math/Vectors/Vec4.h"
#include "Math/Vectors/MatrixTransform.h"
#include "Math/Math.h"
#include <SDL2/SDL.h>

using namespace Neptune;


void FPSController::Move(const Input& _input, Camera& _camera)
{
	const SDL_Event& e = static_cast<SDL_Event>(_input);

	const float DEFAULT_ANGLE = 1.0f;
	const float DEFAULT_STEP = 0.1f;

	const Mat4& orientation_matrix = Transpose(_camera.getOrientation());
	Vec4 step(0.0f, 0.0f, 0.0f, 0.0f);
	float step_distance = DEFAULT_STEP;


	switch (e.key.keysym.scancode)
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
		_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_S:
		step.z() = -step_distance;
		step = orientation_matrix * step;
		_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_A:
		step.x() = -step_distance;
		step = orientation_matrix * step;
		_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_D:
		step.x() = step_distance;
		step = orientation_matrix * step;
		_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_Q:
		step.y() = -step_distance;
		step = orientation_matrix * step;
		_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_E:
		step.y() = step_distance;
		step = orientation_matrix * step;
		_camera.translate(step.x(), step.y(), step.z());
		break;


		// Change orientation
	case SDL_SCANCODE_UP:
		_camera.rotate(DEFAULT_ANGLE, Vec3(1.0f, 0.0f, 0.0f));
		break;

	case SDL_SCANCODE_DOWN:
		_camera.rotate(-DEFAULT_ANGLE, Vec3(1.0f, 0.0f, 0.0f));
		break;

	case SDL_SCANCODE_LEFT:
		_camera.rotate(-DEFAULT_ANGLE, Vec3(0.0f, 1.0f, 0.0f));
		break;

	case SDL_SCANCODE_RIGHT:
		_camera.rotate(DEFAULT_ANGLE, Vec3(0.0f, 1.0f, 0.0f));
		break;
	}
}