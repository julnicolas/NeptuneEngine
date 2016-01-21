#pragma once

#include "StdInterface/Updatable.h"

namespace Neptune
{
	class Camera;

	class TempFPSCameraController: public Updatable
	{
	public:
		TempFPSCameraController(): m_camera(nullptr)                       {}
		TempFPSCameraController(Camera* camera): m_camera(camera)          {}
		~TempFPSCameraController()                                         = default;
		TempFPSCameraController(const TempFPSCameraController&)            = delete;
		TempFPSCameraController& operator=(const TempFPSCameraController&) = delete;

		virtual bool init()      final override  { return true;         }      
		virtual bool update()    final override  { move(); return true; } 
		virtual void terminate() final override  {}     
		
		void bindCamera(Camera* camera) { m_camera = camera; }

	private:
		void move();

		Camera* m_camera;
	};
}