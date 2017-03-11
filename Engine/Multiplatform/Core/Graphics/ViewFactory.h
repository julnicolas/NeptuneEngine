#pragma once

#include "Graphics/GraphicsProgram.h"
#include "Graphics/Renderer.h"
#include "Graphics/Color.h"
#include <vector>
#include <string>

namespace Neptune
{
	class View;

	class ViewFactory
	{
	public:
		ViewFactory()                                                         = default;
		ViewFactory(const char* vertShaderName, const char* fragShaderName);
		virtual ~ViewFactory()                                                = default;
		ViewFactory(const ViewFactory&)                                       = default;
		ViewFactory& operator=(const ViewFactory&)                            = default;

		virtual View* create() =0; // Allocates the view on the heap

	protected:
		std::vector<float>                             m_vertices;
		std::vector<Color>                             m_colors;
		std::string                                    m_vertexShaderName;
		std::string                                    m_fragmentShaderName;
		std::vector<GraphicsProgram::ShaderAttribute> m_shaderAttributes;
		std::vector<GraphicsProgram::UniformVarInput> m_uniformVars;
	};
}