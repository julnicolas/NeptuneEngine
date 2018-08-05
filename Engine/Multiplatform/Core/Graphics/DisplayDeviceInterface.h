#pragma once

#include "System/Type/Integers.h"

namespace Neptune
{
	namespace DisplayDeviceInterface
	{
		typedef void* Handle;
		typedef Handle WindowHandle;
		typedef Handle GraphicalContextHandle;
		const Handle INVALID_HANDLE = nullptr;

		enum class MULTI_SAMPLE_ANTI_ALLIASING : u8
		{
			NONE,	/// No multi-sampling
			X2,		/// Sample twice 
			X4,		/// Sample four times
			X8,		/// Sample eight times
			X16		/// Sample sixteen times
		};

		struct OffScreenRenderingSettings
		{
			MULTI_SAMPLE_ANTI_ALLIASING	m_antiAliasing;			/// Enable anti aliasing through multi-sampling. Default is set to MULTI_SAMPLE_ANTI_ALLIASING::NONE. This value is ignored is off screen rendering is not used.
			u32							m_frameBufferHeight;	/// Set to a value greater than 0 to enable off-screen rendering - the scene is rendered in a custom frame-buffer and then blitted on the default back buffer.
			u32							m_frameBufferWidth;		/// Set to a value greater than 0 to enable off-screen rendering - the scene is rendered in a custom frame-buffer and then blitted on the default back buffer.
			bool						m_enableReversedZ;		/// Use the reversed z technique to deal with depth tests. ATTENTION : off screen rendering must be enabled and your app must provide a compatible projection matrix (see xxxx)

			/// No anti aliasing, no off screen rendering, no reversed z.
			OffScreenRenderingSettings() :
				m_antiAliasing(MULTI_SAMPLE_ANTI_ALLIASING::NONE),
				m_frameBufferHeight(0),
				m_frameBufferWidth(0),
				m_enableReversedZ(false) {}
		};

		/// Creates a window to host the graphical context
		WindowHandle CreateWindow(const char* _name, u32 _width, u32 _height, MULTI_SAMPLE_ANTI_ALLIASING _antiAliasing = MULTI_SAMPLE_ANTI_ALLIASING::NONE, bool _fullScreen = false);

		/// Destructs the window that holds the graphical context
		void DestroyWindow(WindowHandle handle);

		/// Creates the graphical context to map to the main window (created by calling CreateWindow)
		/// \pm1 window handle - necessary to map the graphical context. This value is returned by CreateWindow.
		/// \pm2 openGL minor version, default is set to 3 (the engine uses OpenGL version 4.3 by default)
		/// \pm3 openGL major version, default is set to 4 (the engine uses OpenGL version 4.3 by default)
		/// \pm4 (optional) user settings to instantiate a custom frame buffer 
		/// to use off screen rendering (add multi-sampled anti-aliasing, depth tests using the reversed-z technique....).
		GraphicalContextHandle CreateGraphicalContext(WindowHandle _window, u8 _openGLMinorVersion, u8 _openGLMajorVersion, OffScreenRenderingSettings _userSettings = OffScreenRenderingSettings());
		
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