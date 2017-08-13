#pragma once

#include "Application/App.h"
#include "InputEvent.h"

namespace Neptune
{
	class WinApp: public App
	{
	public:
		WinApp();
		virtual ~WinApp();
		WinApp(const WinApp&)            = delete;
		WinApp& operator=(const WinApp&) = delete;

		bool init()      override;
		void terminate() override;

	protected:
		virtual bool fetchAndCopyFirstInputEvent();
	};

}