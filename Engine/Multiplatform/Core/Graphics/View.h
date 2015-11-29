#pragma once

#include "StdInterface/Updatable.h"
#include "Graphics/GraphicalProgram.h"
#include "Math/Geometry/Transform.h"
#include <vector>

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
		void addUniformVar(const GraphicalProgram::UniformVarInput& u) { m_uniformVars.push_back(u); }

	private:
		Transform                                      m_transform;
		std::vector<GraphicalProgram::UniformVarInput> m_uniformVars;
	};
}