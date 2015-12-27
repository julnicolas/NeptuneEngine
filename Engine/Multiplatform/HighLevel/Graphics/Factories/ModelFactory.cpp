#include "Graphics/Factories/ModelFactory.h"
#include "Graphics/ElementRenderer.h"
#include "Graphics/Shader.h"
#include "Graphics/PLYLoader.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

ModelFactory::ModelFactory(const char* fileName)
{
	initModelData( fileName );
}

ElementView* ModelFactory::create()
{
	ElementView* v = new ElementView;

	// Set the vertex shader's parameters
	PLYLoader::PropertyData* prop = nullptr;

	prop = &m_loader.getPropertyBuffer(PLYLoader::PropertyType::POSITION);
	GraphicalProgram::ShaderAttribute position =
	{
		0,                                // layout
		GraphicalProgram::Types::FLOAT,  // Type
		3,                              // nb components per value
		prop->m_bufferSize,            // data size
		prop->m_buffer                // data
	};
	m_shaderAttributes.push_back(position);

	prop = &m_loader.getPropertyBuffer(PLYLoader::PropertyType::COLOR);
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
	prop = &m_loader.getPropertyBuffer(PLYLoader::PropertyType::INDEX);

	// Create the renderer
	ElementRenderer& renderer = static_cast<ElementRenderer&>( v->getRenderer() );

	renderer.setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES);
	renderer.setNbverticesToRender(m_loader.getNbverticesToRender());
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

void ModelFactory::initModelData(const char* fileName)
{
	// Set resources' name
	m_fileName           = fileName;
	m_vertexShaderName   = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/Display.vert";
	m_fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";

	// Load the model
	m_loader.load(m_fileName.c_str());
}