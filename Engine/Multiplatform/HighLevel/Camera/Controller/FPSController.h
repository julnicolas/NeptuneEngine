#pragma once
#include "Input/InputTypes.h"

namespace Neptune
{
	class Camera;

	namespace FPSController
	{
		void Move(const Input& _input, Camera& _camera);
	}
}