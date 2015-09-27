#ifndef NEPTUNE_APP_H_
#define NEPTUNE_APP_H_

#include "Graphics/Renderer.h"
#include "Input/InputSensitiveController.h"
#include <list>

namespace Neptune
{
	class App
	{
	public:
		App();
		virtual ~App()             = default;
		App(const App&)            = delete;
		App& operator=(const App&) = delete;

		virtual void init() {}
		void execute();
		virtual void terminate() {}

		static InputSensitiveController::ControllerId registerController(InputSensitiveController* const ctrl);
		static void unregisterController(InputSensitiveController* const ctrl);

	protected:
		void executeControllers();
		virtual bool fetchAndCopyFirstInputEvent() = 0;

		InputEvent*                                  m_inputEvent;
		static std::list<InputSensitiveController*>  m_controllers;
		Renderer                                     m_renderer;
	};
}

#endif