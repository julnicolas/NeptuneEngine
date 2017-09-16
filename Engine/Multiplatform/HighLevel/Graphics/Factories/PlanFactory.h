#pragma once

#include "Graphics/ViewFactory.h"
#include "Graphics/Texture.h"
#include "Graphics/VAOView.h"

namespace Neptune
{
	class VAOView;

	class PlanFactory final: public ViewFactory
	{
	public:
		PlanFactory(const Color& color);
		PlanFactory(const char* _texturePath);	

		virtual ~PlanFactory()                     = default;
		PlanFactory(const PlanFactory&)            = delete;
		PlanFactory& operator=(const PlanFactory&) = delete;

		VAOView* create() final override;	/// Allocates a new VAOView on the heap. Class users must handle the object's deallocation.

	private:
		PlanFactory();

		Texture				m_texture;
		std::vector<float>	m_texCoords;
	};
}