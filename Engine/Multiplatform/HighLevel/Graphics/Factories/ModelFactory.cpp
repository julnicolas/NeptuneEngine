#include "Graphics/Factories/ModelFactory.h"
#include "Graphics/ElementRenderer.h"
#include "Graphics/Shader.h"
#include "Graphics/PLYLoader.h"
#include "Graphics/GraphicalEnumMappingFunctions.h"
#include "Graphics/UniformVarNames.h"

#include "Debug/NeptuneDebug.h"
#include "Profiling/Chrono.h"


using namespace Neptune;

ModelFactory::ModelFactory(const char* fileName)
{
	NEP_ASSERT(fileName != nullptr);
	
	// Add vertex shader
	const char VERTEX_SHADER_NAME[] = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/Display.vert";
	Shader vert(VERTEX_SHADER_NAME, GL_VERTEX_SHADER);
	m_program.add(vert.getId());
	
	// Add fragment shader
	const char FRAGMENT_SHADER_NAME[] = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";
	Shader frag(FRAGMENT_SHADER_NAME, GL_FRAGMENT_SHADER);
	m_program.add(frag.getId());
	
#ifdef NEP_DEBUG
	// Set resources' name
	m_dbg_fileName = fileName;
#endif

	// Load model
	m_loader.load(fileName);

	// Set vertex shader's parameters
	PLYLoader::PropertyData* prop = nullptr;

	prop = &m_loader.getPropertyBuffer(PLYLoader::PropertyType::POSITION);
	{
		GraphicsProgram::ShaderAttribute position =
		{
			0,                                // layout
			MapType(prop->m_valueType),		// Type
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

	// Create program
	m_program.addShaderAttribute(m_shaderAttributes[0]);
	m_program.addShaderAttribute(m_shaderAttributes[1]);
	m_program.build();
}

ElementView* ModelFactory::create()
{
	NEP_PROFILING_CHRONO_INIT;
	NEP_PROFILING_CHRONO_START;
	// Create view
	ElementView* v = new ElementView;

	// Add MV matrix
	GraphicsProgram::UniformVarInput mv(NEP_UNIVNAME_MV_MATRIX,
		GraphicsProgram::FLOAT,
		4,
		4,
		16*sizeof(float),
		v->getTransform().getDataPtr());

	m_program.addUniformVariable(mv);

	// Add a default projection
	Mat4 I;
	GraphicsProgram::UniformVarInput projection_matrix(NEP_UNIVNAME_PROJ_MATRIX,
		GraphicsProgram::FLOAT,
		4,
		4,
		16*sizeof(float),
		&I);

	m_program.addUniformVariable(projection_matrix);

	// Get IBO
	PLYLoader::PropertyData* prop = &m_loader.getPropertyBuffer(PLYLoader::PropertyType::INDEX);

	// Set up the view
	v->setDrawingPrimitive(Renderer::DrawingPrimitive::TRIANGLES);
	v->setNbVerticesToRender(m_loader.getNbverticesToRender());
	v->setIndexBufferData( prop->m_buffer, prop->m_bufferSize, ElementRenderer::IndexType::U32 );

	// Add the program to the view
	v->addGraphicsProgram(&m_program);
	
	double t = NEP_PROFILING_CHRONO_STOP;
	NEP_LOG("ModelFactory::create time %f ms", t);

	return v;
}