#pragma once


//////////////////////////////////////////
//
//    This file is being refactored
//
/////////////////////////////////////////


//#include "Graphics/Renderer.h"
#include "StdInterface/Updatable.h"
#include "Graphics/DisplayDeviceInterface.h"
#include "Graphics/Camera.h"
#include "Camera/Controller/TempFPSCameraController.h"
#include <vector>


namespace Neptune
{
	class View;

	class App : public Neptune::Updatable
	{
	public:
		App();
		virtual ~App()             = default;
		App(const App&)            = delete;
		App& operator=(const App&) = delete;

		void setWindowSize(u32 _height, u32 _width)	{ m_windowHeight = _height; m_windowWidth = _width; }

		bool init()      override;     /// Prepares the app to be updated.
		bool update()    override;    /// Executed once per frame.
		void terminate() override;   /// Must be executed when the app exits from the main loop.

	protected:
		u32												m_windowHeight;
		u32												m_windowWidth;
		DisplayDeviceInterface::WindowHandle			m_window;
		DisplayDeviceInterface::GraphicalContextHandle	m_context;
		Camera											m_camera;
		TempFPSCameraController							m_controller;
		std::vector<View*>								m_views;
	};
}
