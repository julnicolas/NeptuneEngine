#pragma once

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
		App(const char* _appName);
		virtual ~App();
		App(const App&)            = delete;
		App& operator=(const App&) = delete;


		////////////////////////////////////////////////////////////////
		//
		//			P R E  -  I N I T   C A L L S
		//
		////////////////////////////////////////////////////////////////

		void setWindowSize(u32 _height, u32 _width)	{ m_windowHeight = _height; m_windowWidth = _width; }
		
		/// Enable reversed-z technique.
		/// \brief This feature enables large-view-rendering with few z-fighting issues.
		/// \warning This technique is only available if GL_ARB_CLIP_CONTROL or OpenGL 4.5 is supported.
		/// \note Nvidia and ATI cards support this feature since 2014 on Windows, 2015 on Linux.
		/// For Intel, support for this feature starts in 2016.
		bool enableReversedZ();

		// Projection matrices
		void useStandardPerspectiveMatrix();			/// Standard perspective projective matrix which projects scene's content between the near and far planes.
		void useReversedZPerspectiveMatrix();			/// Set perspective projection matrix to be compatible with the reversed-z technique. Scene is projected between the near and far planes.
		void useReversedZInfinityPerspectiveMatrix();	/// Set perspective projection matrix to be compatible with the reversed-z technique. Scene points to infinity.


		////////////////////////////////////////////////////////////////
		//
		//			A P P   S T A T E   M A N A G E M E N T
		//
		////////////////////////////////////////////////////////////////

		bool init()      override;						/// Prepares the app to be updated.
		bool update()    override;						/// Executed once per frame.
		void terminate() override;						/// Must be executed when the app exits from the main loop.

	protected:
		// User frame control
		virtual bool onFrameStart();					/// Called before anything else in the main loop
		virtual bool onFrameEnd();						/// Last function called in the main loop
		virtual bool forEachViewDo(View* _view);		/// Called for each view, right before View::update() is called.

		const char*										m_appName;
		u32												m_windowHeight;
		u32												m_windowWidth;
		DisplayDeviceInterface::WindowHandle			m_window;
		DisplayDeviceInterface::GraphicalContextHandle	m_context;
		Camera											m_camera;
		TempFPSCameraController							m_controller;
		std::vector<View*>								m_views;
	};
}
