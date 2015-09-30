#pragma once

#include "System/NeptuneTypes.h"

namespace Neptune
{
	namespace DisplayDeviceInterface
	{
		typedef void* WindowHandle;
		typedef void* GraphicalContextHandle;

		// Create a window to host the graphical context
		WindowHandle CreateWindow(const char* name, u32 width, u32 height, bool fullScreen = false);

		// Destruct the window that holds the graphical context
		void DestroyWindow(WindowHandle handle);

		// Creates a graphical context as well as the app's main window
		GraphicalContextHandle CreateGraphicalContext(WindowHandle window, u8 minCtxtVersion,u8 maxCtxtVersion);
		
		// Destruct a graphical context and the main window
		void DestroyGraphicalContext(GraphicalContextHandle handle);

		// Swap the front and the back buffer when double buffering is enabled
		void SwapBuffer(WindowHandle handle);

		// Getters
		u8 GetDepthBufferLength(); // Result in bits
	}
}