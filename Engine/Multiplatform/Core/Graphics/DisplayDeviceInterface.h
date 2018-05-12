#pragma once

#include "System/Type/Integers.h"

namespace Neptune
{
	namespace DisplayDeviceInterface
	{
		typedef void* WindowHandle;
		typedef void* GraphicalContextHandle;

		enum class MULTI_SAMPLE_ANTI_ALLIASING : u8
		{
			NONE,	/// No multi-sampling
			X2,		/// Sample twice 
			X4,		/// Sample four times
			X8,		/// Sample eight times
			X16		/// Sample sixteen times
		};

		/// Creates a window to host the graphical context
		WindowHandle CreateWindow(const char* _name, u32 _width, u32 _height, MULTI_SAMPLE_ANTI_ALLIASING _antiAliasing = MULTI_SAMPLE_ANTI_ALLIASING::NONE, bool _fullScreen = false);

		/// Destructs the window that holds the graphical context
		void DestroyWindow(WindowHandle handle);

		/// Creates a graphical context as well as the app's main window
		GraphicalContextHandle CreateGraphicalContext(WindowHandle window, u8 minCtxtVersion,u8 maxCtxtVersion, bool _setReversedZ = false);
		
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