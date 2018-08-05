#pragma once

#include "Input/InputEvent.h"
#include "System/Type/Integers.h"

namespace Neptune
{
	class InputSensitiveController
	{
	public:
		typedef u8 ControllerId;

		InputSensitiveController()                                                   = default;
		virtual ~InputSensitiveController()                                          = default;
		InputSensitiveController(const InputSensitiveController&)                    = default;
		virtual InputSensitiveController& operator=(const InputSensitiveController&) = default;

		void execute(InputEvent* e) { process(e); }

	protected:
		virtual void process(InputEvent* e) = 0;
	};
}