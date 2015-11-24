#pragma once

#include "StdInterface/Updatable.h"
#include "Graphics/GraphicalProgram.h"
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

		virtual Renderer& getRenderer() =0;
		void addUniformVar(const GraphicalProgram::UniformVarInput& u) { m_uniformVars.push_back(u); }

	private:
		std::vector<GraphicalProgram::UniformVarInput> m_uniformVars;
	};
}