#pragma once

#include "System/NeptuneTypes.h"

namespace Neptune
{
	namespace GraphicalContext
	{
		// Creates a graphical context as well as the app's main window
		bool CreateGraphicalContext(const char* windowName, u16 width,u16 height,u8 minCtxtVersion,u8 maxCtxtVersion);
		
		// Destruct a graphical context and the main window
		void DestructGraphicalContext();

		// Swap the front and the back buffer when double buffering is enabled
		void SwapBuffer();

		// Getters
		u8 GetDepthBufferLength();
	}
}