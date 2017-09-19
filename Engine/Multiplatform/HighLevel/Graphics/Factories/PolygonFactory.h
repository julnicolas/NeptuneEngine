#pragma once

#include "Graphics/Factories/ViewFactory.h"
#include "Graphics/Texture.h"

namespace Neptune
{
	class PolygonFactory : public ViewFactory
	{
	public:
		PolygonFactory(const Color& _color);
		PolygonFactory(const char* _texturePath);

		virtual ~PolygonFactory()									= default;
		PolygonFactory(const PolygonFactory&)						= delete;
		virtual PolygonFactory& operator=(const PolygonFactory&)	= delete;

		View* create() final override;

	protected:
		/// \brief In this method you must dynamically allocate a View object
		/// (called v). Then call v->getRenderer().setDrawingPrimitive(_prim)  
		/// and v->getRenderer().setNbverticesToRender(nb).
		virtual View* createViewAndSetUpRenderParameters()	=0;

		virtual void createVertexData()			= 0;
		virtual void createTextureCoordinates()	= 0;

		Texture				m_texture;
		std::vector<float>	m_texCoords;

	private:
		void initPolygonData();
	};
}

