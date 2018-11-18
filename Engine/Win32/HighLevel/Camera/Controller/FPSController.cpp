#include "Camera/Controller/FPSController.h"
#include "Input/InputTypes.h"
#include "Graphics/Camera.h"
#include "Math/Vectors/Mat4x4.h"
#include "Math/Vectors/Vec4.h"
#include "Math/Vectors/MatrixTransform.h"
#include "Math/Math.h"
#include <SDL2/SDL.h>

using namespace Neptune;


const float DEFAULT_STEP = 0.1f; // Default distance used to move the camera

FPSController::FPSController(Camera& _camera) : 
	m_camera(_camera),
	m_stepDistance(DEFAULT_STEP)
{

}


void FPSController::operator()(const Input& _input)
{
	const SDL_Event& e = static_cast<SDL_Event>(_input);

	const float DEFAULT_ANGLE = 1.0f;

	const Mat4& orientation_matrix = Transpose(m_camera.getOrientation());
	Vec4 step(0.0f, 0.0f, 0.0f, 0.0f);

	switch (e.key.keysym.scancode)
	{
		// Change acceleration
	case SDL_SCANCODE_Z:
		m_stepDistance += 0.1f;
		break;

	case SDL_SCANCODE_X:
		m_stepDistance = Max(0.1f, m_stepDistance - 0.1f);
		break;

		// Change position
	case SDL_SCANCODE_W:
		step.z() = m_stepDistance;
		step = orientation_matrix * step;
		m_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_S:
		step.z() = -m_stepDistance;
		step = orientation_matrix * step;
		m_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_A:
		step.x() = -m_stepDistance;
		step = orientation_matrix * step;
		m_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_D:
		step.x() = m_stepDistance;
		step = orientation_matrix * step;
		m_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_Q:
		step.y() = -m_stepDistance;
		step = orientation_matrix * step;
		m_camera.translate(step.x(), step.y(), step.z());
		break;

	case SDL_SCANCODE_E:
		step.y() = m_stepDistance;
		step = orientation_matrix * step;
		m_camera.translate(step.x(), step.y(), step.z());
		break;


		// Change orientation
	case SDL_SCANCODE_UP:
		m_camera.rotate(DEFAULT_ANGLE, Vec3(1.0f, 0.0f, 0.0f));
		break;

	case SDL_SCANCODE_DOWN:
		m_camera.rotate(-DEFAULT_ANGLE, Vec3(1.0f, 0.0f, 0.0f));
		break;

	case SDL_SCANCODE_LEFT:
		m_camera.rotate(-DEFAULT_ANGLE, Vec3(0.0f, 1.0f, 0.0f));
		break;

	case SDL_SCANCODE_RIGHT:
		m_camera.rotate(DEFAULT_ANGLE, Vec3(0.0f, 1.0f, 0.0f));
		break;
	}
}