#include "Graphics/Factories/ViewSpawner.h"
#include "Graphics/View.h"
#include "Physics/Mechanics/Position.h"
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

//////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// GL LAYOUT PROBLEM FOR THE GRAPHICS PROGRAM ROUTINES
//
//////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



static void InitWorldMatrix(float (&_m)[4][4])
{
	memset(_m, 0, sizeof(_m));
	
	// Making it an identity matrix
	_m[0][0] = 1.0f;
	_m[1][1] = 1.0f;
	_m[2][2] = 1.0f;
	_m[3][3] = 1.0f;
}

ViewSpawner::ViewSpawner(GraphicsProgram* _pgm)
{
	// A position is 3 float-long data and m_worldMatrix is a float[4][4]
	NEP_STATIC_ASSERT(sizeof(Position) == 3*sizeof(m_worldMatrix[0][0]), "Error: type mismatch (m_worldMatrix's data and Position's members)");
	InitWorldMatrix(m_worldMatrix);
	addGraphicsProgram(_pgm);
}

View* ViewSpawner::create()
{
	View* view = createViewAndSetUpRenderParameters();

	// Add the graphics programs and corresponding attributes to the view
	for(auto& it : m_programs)
	{
		// Pass the parameters to the program
		GraphicsProgram* pgm = it.second.m_program;

		for(const auto& shader_attribute_ID : it.second.m_shaderAttributeIDs)
		{
			NEP_ASSERT(m_shaderAttributes.find(shader_attribute_ID) != m_shaderAttributes.end());
			pgm->addShaderAttribute(m_shaderAttributes[shader_attribute_ID]);
		}

		for(const auto& uniform_var_ID : it.second.m_uniformVarIDs)
		{
			NEP_ASSERT(m_uniformVariables.find(uniform_var_ID) != m_uniformVariables.end());
			pgm->addUniformVariable(m_uniformVariables[uniform_var_ID]);
		}

		// Rebuild the program including all its parameters
		pgm->build();

		// Add the program to the view
		view->addGraphicsProgram(pgm);
	}

	return view;
}

void ViewSpawner::addGraphicsProgram(GraphicsProgram* _pgm)
{
	NEP_ASSERT(_pgm != nullptr);
	
	// Create a Program struct
	Program program;
	program.m_program = _pgm;

	// Add it to the program list if not present, otherwise do nothing
	m_programs.insert({ _pgm->getName(), program});
}

void ViewSpawner::addShaderAttribute(GraphicsProgram::ProgramName _pgmName, const GraphicsProgram::ShaderAttribute& _shaderAtt)
{
	// Get the program
	auto it = m_programs.find(_pgmName);
	NEP_ASSERT( it != m_programs.end() );

	// Add the attribute if not already present
	const void* attribute_ID = _shaderAtt.m_data;
	m_shaderAttributes.insert({attribute_ID,_shaderAtt});

	// Add its ID to the corresponding program
	it->second.m_shaderAttributeIDs.push_back(attribute_ID);
}

void ViewSpawner::addUniformVariable(GraphicsProgram::ProgramName _pgmName, const GraphicsProgram::UniformVarInput& _uniform)
{
	// Get the program
	auto it = m_programs.find(_pgmName);
	NEP_ASSERT(it != m_programs.end());

	// Add the attribute if not already present
	const void* uniform_ID = _uniform.getData();
	m_uniformVariables.insert({uniform_ID,_uniform});

	// Add its ID to the corresponding program
	it->second.m_uniformVarIDs.push_back(uniform_ID);
}

bool ViewSpawner::mapVertexData(GraphicsProgram::ProgramName _pgmName, u8 _layout)
{
	// Check the data have been created
	if(m_vertices.empty()) // Assert?
		return false;

	// Create the shader attribute
	GraphicsProgram::ShaderAttribute att;
	att.m_layout		= _layout;
	att.m_type			= GraphicsProgram::FLOAT;
	att.m_nbComponents	= 3;
	att.m_normalized	= false;
	att.m_size			= m_vertices.size()*sizeof(m_vertices[0]);
	att.m_data			= m_vertices.data();

	// Add the attribute to the program
	addShaderAttribute(_pgmName,att);

	return true;
}

bool ViewSpawner::mapColorData(GraphicsProgram::ProgramName _pgmName, u8 _layout)
{
	// Check the data have been created
	if ( m_colors.empty() ) // Assert?
		return false;
	
	// Create the shader attribute
	GraphicsProgram::ShaderAttribute att;
	att.m_layout		= _layout;
	att.m_type			= GraphicsProgram::FLOAT;
	att.m_nbComponents	= 4;
	att.m_normalized	= false;
	att.m_size			= m_colors.size()*sizeof(m_colors[0]);
	att.m_data			= m_colors.data();

	// Add the attribute to the program
	addShaderAttribute(_pgmName, att);
	
	return true;
}

bool ViewSpawner::mapNormalData(GraphicsProgram::ProgramName _pgmName, u8 _layout)
{
	if(m_normals.empty())
		return false;
	
	// Create the shader attribute
	GraphicsProgram::ShaderAttribute att;
	att.m_layout		= _layout;
	att.m_type			= GraphicsProgram::FLOAT;
	att.m_nbComponents	= 3;
	att.m_normalized	= false;
	att.m_size			= m_normals.size()*sizeof(m_normals[0]);
	att.m_data			= m_normals.data();

	// Add the attribute to the program
	addShaderAttribute(_pgmName,att);

	return true;
}

bool ViewSpawner::map2DTextureMapData(GraphicsProgram::ProgramName _pgmName, u8 _layout)
{
	if(m_2DTexCoords.empty())
		return false;
	
	// Create the shader attribute
	GraphicsProgram::ShaderAttribute att;
	att.m_layout		= _layout;
	att.m_type			= GraphicsProgram::FLOAT;
	att.m_nbComponents	= 2;
	att.m_normalized	= false;
	att.m_size			= m_2DTexCoords.size()*sizeof(m_2DTexCoords[0]);
	att.m_data			= m_2DTexCoords.data();

	// Add the attribute to the program
	addShaderAttribute(_pgmName,att);

	return true;
}

void ViewSpawner::setWorldPosition(const Position& _pos)
{
	// Filling the matrix's fields to represent a translation
	// Note: OpenGL's matrices are all transposed with respect to 
	// the common mathematical convention
	m_worldMatrix[3][0] = _pos.m_x;
	m_worldMatrix[3][1] = _pos.m_y;
	m_worldMatrix[3][2] = _pos.m_z;
	// NEP_STATIC_ASSERT(sizeof(m_worldMatrix[3] >= sizeof(_pos)), "Error: memcpy overflow")
	// memcpy(m_worldMatrix[3], _pos, sizeof(_pos));
}

void ViewSpawner::useWorldMatrix(GraphicsProgram::ProgramName _pgmName, const char* _uniformName)
{
	GraphicsProgram::UniformVarInput world_matrix(_uniformName,
		GraphicsProgram::FLOAT,
		4,
		4,
		sizeof(m_worldMatrix),
		&m_worldMatrix);

	addUniformVariable(_pgmName, world_matrix);
}