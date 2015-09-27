#pragma once

#include "Application/App.h"
#include "Plateform.h"

namespace Neptune
{
	class WinApp: public App
	{
	public:
		WinApp();
		virtual ~WinApp();
		WinApp(const WinApp&)            = delete;
		WinApp& operator=(const WinApp&) = delete;

		virtual void init();
		virtual void terminate();

	protected:
		virtual bool fetchAndCopyFirstInputEvent();
	};

}