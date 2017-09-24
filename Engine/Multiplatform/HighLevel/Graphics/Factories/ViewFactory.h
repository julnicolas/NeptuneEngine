#pragma once

#include "Graphics/GraphicsProgram.h"
#include "Graphics/Color.h"

#include <vector>

namespace Neptune
{
	class View;
	class Texture;

	/// \class This class allows users to instantiate a view in a really few lines of code.
	/// \example 
	/// PlanFactory factory("Resources/Textures/Grass.png");
	/// View* v1 = factory.create();
	/// v1->init(); // don't forget to init the view before calling update
	///
	/// \warning The instantiating object must remain in memory for the views to be rendered. 
	/// The views directly depend on this object.
	///
	/// For a more efficient implementation please check the ViewSpawner class hierarchy.

	class ViewFactory
	{
	public:
		ViewFactory(){}

		virtual ~ViewFactory()                                                = default;
		ViewFactory(const ViewFactory&)                                       = default;
		ViewFactory& operator=(const ViewFactory&)                            = default;

		virtual View* create() =0; // Allocates the view on the heap

	protected:
		GraphicsProgram									m_program;
		std::vector<float>								m_vertices;
		std::vector<Color>								m_colors;
		std::vector<GraphicsProgram::ShaderAttribute>	m_shaderAttributes;
		std::vector<GraphicsProgram::UniformVarInput>	m_uniformVars;
	};
}