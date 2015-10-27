#pragma once

#include "System/Type/Integers.h"

namespace Neptune
{
	namespace DisplayDeviceInterface
	{
		typedef void* WindowHandle;
		typedef void* GraphicalContextHandle;

		/// Creates a window to host the graphical context
		WindowHandle CreateWindow(const char* name, u32 width, u32 height, bool fullScreen = false);

		/// Destructs the window that holds the graphical context
		void DestroyWindow(WindowHandle handle);

		/// Creates a graphical context as well as the app's main window
		GraphicalContextHandle CreateGraphicalContext(WindowHandle window, u8 minCtxtVersion,u8 maxCtxtVersion);
		
		/// Destructs a graphical context and the main window
		void DestroyGraphicalContext(GraphicalContextHandle handle);

		/// Clears any relevant buffer before the rendering stage
		void ClearBuffers(float backGroundColor[4]);

		/// Swaps the front and the back buffer when double buffering is enabled
		void SwapBuffer(WindowHandle handle);

		// Getters
		u8 GetDepthBufferLength(); // Result in bits

	}
}