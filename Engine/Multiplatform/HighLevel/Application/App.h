#pragma once


//////////////////////////////////////////
//
//    This file is being refactored
//
/////////////////////////////////////////


//#include "Graphics/Renderer.h"
#include "StdInterface/Updatable.h"
#include "Input/InputSensitiveController.h"

#include <list> // Perhaps this should be reconsidered

namespace Neptune
{
	class App : public Neptune::Updatable
	{
	public:
		App();
		virtual ~App()             = default;
		App(const App&)            = delete;
		App& operator=(const App&) = delete;

		bool init()      override;     /// Prepares the app to be updated.
		bool update()    override;    /// Executed once per frame.
		void terminate() override;   /// Must be executed when the app exits from the main loop.

		static InputSensitiveController::ControllerId registerController(InputSensitiveController* const ctrl);
		static void unregisterController(InputSensitiveController* const ctrl);

	protected:
		//void executeControllers();
		//virtual bool fetchAndCopyFirstInputEvent() = 0;

		InputEvent*                                  m_inputEvent; // To be refactored
		static std::list<InputSensitiveController*>  m_controllers;// To be refactored
		//Renderer                                     m_renderer;   // Should be replaced by a collection of Drawables
	};
}
