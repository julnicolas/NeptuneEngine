#include "Graphics/Factories/ModelFactory.h"
#include "Graphics/ElementRenderer.h"
#include "Graphics/Shader.h"
#include "Graphics/PLYLoader.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

ModelFactory::ModelFactory()
{
	initCubeData(0.5f,0.5f,0.5f);
}

ModelFactory::ModelFactory(float r,float g,float b)
{
	initCubeData(r,g,b);
}

ElementView* ModelFactory::create()
{
	ElementView* v = new ElementView;

	// Load model
	PLYLoader* p_loader = new PLYLoader;
	PLYLoader& loader = *p_loader;
	loader.load("Resources/Models/xwing.ply");

	// Set vertex shader's parameters
	PLYLoader::PropertyData* prop = nullptr;

	prop = &loader.getPropertyBuffer(PLYLoader::PropertyType::POSITION);
	GraphicalProgram::ShaderAttribute position =
	{
		0,                                // layout
		GraphicalProgram::Types::FLOAT,  // Type
		3,                              // nb components per value
		prop->m_bufferSize,            // data size
		prop->m_buffer                // data
	};
	m_shaderAttributes.push_back(position);

	prop = &loader.getPropertyBuffer(PLYLoader::PropertyType::COLOR);
	GraphicalProgram::ShaderAttribute color =
	{
		1,                                      // layout
		GraphicalProgram::Types::U8,          // Type
		3,                                    // nb components per value
		prop->m_bufferSize,					 // data size
		prop->m_buffer                      // data
	};
	m_shaderAttributes.push_back(color);

	// Add the MV matrix

	GraphicalProgram::UniformVarInput mv("ModelView",
		GraphicalProgram::FLOAT,
		4,
		4,
		16*sizeof(float),
		v->getTransform().getDataPtr());

	// Get the IBO
	prop = &loader.getPropertyBuffer(PLYLoader::PropertyType::INDEX);

	// Create the renderer
	ElementRenderer& renderer = static_cast<ElementRenderer&>( v->getRenderer() );

	renderer.setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES);
	renderer.setNbverticesToRender(loader.getNbverticesToRender());
	renderer.setIndexBufferData( prop->m_buffer, prop->m_bufferSize, ElementRenderer::IndexType::U32 );

	// Create the shaders to display the model
	Shader vert(m_vertexShaderName.c_str(),GL_VERTEX_SHADER);
	Shader frag(m_fragmentShaderName.c_str(),GL_FRAGMENT_SHADER);

	// Create the program to display a triangle
	{
		GraphicalProgram& pgm = renderer.createProgram();
		pgm.add(vert.getId());
		pgm.add(frag.getId());
		pgm.addShaderAttribute(m_shaderAttributes[0]);
		pgm.addShaderAttribute(m_shaderAttributes[1]);
		pgm.addUniformVariable(mv);
		pgm.build();
	}

	return v;
}

void ModelFactory::initCubeData(float r,float g,float b)
{
	// Set vertex data
	/*float t1[] = {-1.0f, 0.0f, 0.1f, 0.0f, 1.0f, 0.1f, 1.0f, 0.0f, 0.1f};

	for(unsigned int i = 0; i < sizeof(t1)/sizeof(float); i++)
		m_vertices.push_back(t1[i]);*/

	// Create shader attributes

	/*GraphicalProgram::ShaderAttribute t1_data =
	{
		0,                                // layout
		GraphicalProgram::Types::FLOAT,  // Type
		3,                              // nb components per value
		sizeof(t1),                    // data size
		&m_vertices[0]                // data
	};

	GraphicalProgram::ShaderAttribute c1_data =
	{
		1,                                      // layout
		GraphicalProgram::Types::FLOAT,        // Type
		3,                                    // nb components per value
		m_colors.size()*sizeof(m_colors[0]), // data size
		&m_colors[0]                        // data
	};*/

	// Add them to the factory
	//m_shaderAttributes.push_back(t1_data);
	//m_shaderAttributes.push_back(c1_data);

	// Set shader names
	m_vertexShaderName   = "Resources/Shaders/SelfPractice/DisplayWithDividedColors.vert";
	m_fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";
}