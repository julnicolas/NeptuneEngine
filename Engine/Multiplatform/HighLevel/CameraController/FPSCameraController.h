#pragma once

#include "Input/InputSensitiveController.h"

namespace Neptune
{
	class Camera;

	class FPSCameraController: public InputSensitiveController
	{
	public:
		FPSCameraController();
		FPSCameraController(Camera* c);
		virtual ~FPSCameraController()  = default;

		FPSCameraController(const FPSCameraController&)            = delete;
		FPSCameraController& operator=(const FPSCameraController&) = delete;

	protected:
		virtual void process(InputEvent*);

	private:
		Camera* m_camera;
	};


}