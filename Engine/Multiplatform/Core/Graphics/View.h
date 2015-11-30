#pragma once

#include "StdInterface/Updatable.h"
#include "Graphics/GraphicalProgram.h"
#include "Math/Geometry/Transform.h"
#include <unordered_map>

namespace Neptune
{
	class Renderer;

	class View : public Updatable
	{
	public:
		View()                       = default;
		virtual ~View()              {}
		View(const View&)            = delete;
		View& operator=(const View&) = delete;

		Transform& getTransform()   { return m_transform; } 
		virtual Renderer& getRenderer() =0;

	protected:
		Transform      m_transform;
	};
}