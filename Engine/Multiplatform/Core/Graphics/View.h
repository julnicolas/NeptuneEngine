#pragma once

#include "StdInterface/Updatable.h"
#include "Math/Geometry/Transform.h"
#include "Graphics/Renderer.h"

#include <unordered_map>

namespace Neptune
{
	class Camera;
	class Texture;

	class View : public Updatable
	{
	public:
		View(): m_camera(nullptr),
			    m_renderer(nullptr),
				m_texture(nullptr)   {}
		virtual ~View()              {}
		View(const View&)            =		delete;
		View& operator=(const View&) =		delete;

		bool		init()					override;
		bool		update()				override;
		void		terminate()				override;

		Transform&	getTransform()			{ return m_transform; }

		bool		updateUniform(const char* _pgmName, const char* _uniformName, void* _value);	/// Updates _uniformName with _value in program _pgmName. Returns true, if the uniform was found, false otherwise.
		bool		updateUniform(const char* _uniformName, void* _value);							/// Updates _uniformName for every program with value _value. Returns true if it has been updated at least once. False means the variable doesn't exist for any program.
		void		addGraphicsProgram(const char* _pgmName, GraphicsProgram* _pgm);				/// \warning _pgm must be available (not deallocated) for the entire lifetime of the view object. _pgm is referenced not copied.

		void		bindToCamera(Camera* c) { m_camera = c;       }
		void		unbindFromCamera()      { m_camera = nullptr; }

		void		bindTexture(Texture* t) { m_texture = t;       }
		void		unbindTexture()         { m_texture = nullptr; }

		void		setDrawingPrimitive(Renderer::DrawingPrimitive _prim)	{ m_renderer->setDrawingPrimitive(_prim); }
		void		setNbVerticesToRender(u32 _nb)							{ m_renderer->setNbverticesToRender(_nb); }

		// Implemented to fix a warning stating this class should be 16-bit aligned
		// Further investigation would be appreciated though
		void* operator new(std::size_t count);
		void  operator delete(void* ptr);

	protected:
		Transform												m_transform;
		Camera*													m_camera;
		Renderer*												m_renderer;
		std::unordered_map<const char*, Renderer::ProgramID>	m_programMap;
		Texture*												m_texture;
	};
}