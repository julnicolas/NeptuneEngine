#pragma once
#include "Input/InputTypes.h"


namespace Neptune
{
	class SimpleApp;

	namespace WindowController
	{
		/// Closes _app's main window
		/// \param _input Window input fetched from an InputProducer
		/// \param _app Pointer to a SimpleApp instance.
		void Close(const Input& _input, SimpleApp* _app);
	}
}