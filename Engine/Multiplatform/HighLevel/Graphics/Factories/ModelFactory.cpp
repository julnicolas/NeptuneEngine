#include "Graphics/Factories/ModelFactory.h"
#include "Graphics/ElementRenderer.h"
#include "Graphics/Shader.h"
#include "Graphics/PLYLoader.h"
#include "Graphics/GraphicalEnumMappingFunctions.h"
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
	{
		GraphicsProgram::ShaderAttribute position =
		{
			0,                                // layout
			MapType(prop->m_valueType),  // Type
			3,                              // nb components per value
			false,                          // Should data be normalized?
			prop->m_bufferSize,            // data size
			prop->m_buffer                // data
		};
		m_shaderAttributes.push_back(position);
	}

	prop = &m_loader.getPropertyBuffer(PLYLoader::PropertyType::COLOR);
	{
		GraphicsProgram::ShaderAttribute color;
		
		color.m_layout = 1;
		color.m_type = MapType( prop->m_valueType );
		color.m_nbComponents = 3;
		color.m_normalized = (prop->m_valueType != PLYLoader::FLOAT && prop->m_valueType != PLYLoader::DOUBLE) ? true : false;
		color.m_size = prop->m_bufferSize;
		color.m_data = prop->m_buffer;
		
		m_shaderAttributes.push_back(color);
	}

	// Add the MV matrix
	GraphicsProgram::UniformVarInput mv("ModelView",
		GraphicsProgram::FLOAT,
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

	// Create the program
	{
		GraphicsProgram& pgm = renderer.createProgram();
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