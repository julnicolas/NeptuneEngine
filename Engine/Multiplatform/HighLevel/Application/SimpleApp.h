#pragma once

#include "Graphics/DisplayDeviceInterface.h"
#include "Graphics/Camera.h"
#include "Graphics/Color.h"
#include "Input/InputConsumer.h"
#include <vector>
#include <functional>


namespace Neptune
{
	class View;
	class InputProducer;

	class SimpleApp
	{
	public:
		SimpleApp(u32 _windowWidth, u32 _windowHeight, const char* _appName, bool _fullScreen = false);
		virtual ~SimpleApp();
		SimpleApp(const SimpleApp&)            = delete;
		SimpleApp& operator=(const SimpleApp&) = delete;


		////////////////////////////////////////////////////////////////
		//
		//			M A I N   L O O P   M A N A G E M E N T
		//
		////////////////////////////////////////////////////////////////

		void loop();									/// Executes the main loop
		void stopUpdate() {								/// Stops the execution of the main loop
			m_is_update_enabled = false; 
		}

		void resumeUpdate() {							/// Resumes the execution of the main loop
			m_is_update_enabled = true;
		}

		////////////////////////////////////////////////////////////////
		//
		//			D A T A   M A N A G E M E N T
		//
		////////////////////////////////////////////////////////////////

		void add(View* _view);
		// void addViewList(View** _list, u32 _length);  // Must change how views are bound to cameras
		void setBackgroundColor(const Color& _color) { m_backgroundColor = _color;  }
		Camera& getCamera()							 { return m_camera;				}

		///////////////////////////////////////////////////////////////
		//
		//		FRAME MANAGEMENT
		//
		//////////////////////////////////////////////////////////////
		struct FrameData
		{
			Camera* m_camera;					/// reference to the app's camera
			std::vector<View*>* m_views;		/// reference to the app's view vector
		};

		using FrameCallBack = std::function<bool(FrameData)>;

		void setFrameStartCallBack(FrameCallBack _callBack)						{ m_onFrameStartCallBack = _callBack;	 }
		void setFrameEndCallback(FrameCallBack _callBack)						{ m_onFrameEndCallBack   = _callBack;	 }
		void setOnViewUpdateCallBack(std::function<bool(View*)> _callback)		{ m_onViewUpdateCallBack = _callback;	 }

	protected:
		// User frame control
		bool onFrameStart();							/// Called before anything else in the main loop
		bool onFrameEnd();								/// Last function called in the main loop
		bool onViewUpdate(View* _view);				/// Called for each view, right before View::update() is called.

		
		bool											m_is_update_enabled;
		Color											m_backgroundColor;
		FrameCallBack									m_onFrameStartCallBack;
		InputProducer*									m_inputProducer;
		InputConsumer									m_inputConsumer;
		Camera											m_camera;
		std::function<bool(View*)>						m_onViewUpdateCallBack;
		std::vector<View*>								m_views;
		DisplayDeviceInterface::WindowHandle			m_window;
		DisplayDeviceInterface::GraphicalContextHandle	m_context;
		FrameCallBack									m_onFrameEndCallBack;
	};
}
