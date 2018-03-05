#include "Graphics/Spawners/ViewSpawner.h"
#include "Graphics/View.h"
#include "Graphics/Texture.h"
#include "Graphics/UniformVarNames.h"
#include "Physics/Mechanics/Position.h"
#include "Debug/NeptuneDebug.h"
#include "System/Hashing/FastHashFunctions.h"

#include "Profiling/Chrono.h"

using namespace Neptune;

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
	// Create the view
	View* view = createViewAndSetUpRenderParameters();

	// Add the program to the view
	for(auto& it : m_programs)
	{
		GraphicsProgram* pgm = it.second.m_program;

		NEP_ASSERT_ERR_MSG(pgm->getNbVertexAttributes() > 0, "Program doesn't have any vertex attributes... \nDid you call ViewSpawner::movePgmParameters() before ViewSpawner::create()?");
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

	// Set its custom data for the corresponding program
	Program::CustomShaderAttributeData custom_data;
	custom_data.m_id		= attribute_ID;
	custom_data.m_layout	= _shaderAtt.m_layout;

	it->second.m_shaderAttributesCustomData.push_back(custom_data);
}

void ViewSpawner::setTexture(GraphicsProgram::ProgramName _pgmName, Texture* _texture)
{
	NEP_ASSERT(_texture != nullptr); // Error invalid texture
	
	// Get the program
	auto it = m_programs.find(_pgmName);
	NEP_ASSERT( it != m_programs.end() );

	// Add its ID to the corresponding program
	it->second.m_textures[_texture->getIndex()] = _texture;
}

static u64 ComputeUniformVariableId(GraphicsProgram::ProgramName _pgmName, const char* _uniformName)
{
	u32 name_hash = Fnv1a32((u8*) _uniformName, strlen(_uniformName));
	u32 pgm_name  = static_cast<u32>( _pgmName );

	u64 uniformID = 0;			// 0x00000000 00000000
	uniformID     = pgm_name;	// 0x00000000 pgm_name
	uniformID     <<= 32;		// 0xpgm_name 00000000
	uniformID     |= name_hash; // 0xpgm_name name_hash

	return uniformID;
}

void ViewSpawner::addUniformVariable(GraphicsProgram::ProgramName _pgmName, const GraphicsProgram::UniformVarInput& _uniform)
{
	// Get the program
	auto it = m_programs.find(_pgmName);
	NEP_ASSERT(it != m_programs.end());

	// Add the attribute if not already present
	UniformVariableID uniform_ID {ComputeUniformVariableId(_pgmName, _uniform.getName())}; // Don't allow narrowing conversions
	m_uniformVariables.insert({uniform_ID,_uniform});

	// Add its ID to the corresponding program
	it->second.m_uniformVarIDs.push_back(uniform_ID);
}

void ViewSpawner::addUniformVariable(GraphicsProgram::ProgramName* _pgmNameList, u32 _nbPgm, const GraphicsProgram::UniformVarInput& _uniform)
{
	NEP_ASSERT( _pgmNameList != nullptr );
	NEP_ASSERT( _nbPgm > 0 );

	// Add the first element (this one will be shared by the other programs)
	
		// Get the program
		auto it_first = m_programs.find(_pgmNameList[0]);
		NEP_ASSERT(it_first != m_programs.end());

		// Add the attribute if not already present
		UniformVariableID uniform_ID{ComputeUniformVariableId(_pgmNameList[0],_uniform.getName())}; // Don't allow narrowing conversion
		m_uniformVariables.insert({uniform_ID,_uniform});

		// Add its ID to the corresponding program
		it_first->second.m_uniformVarIDs.push_back(uniform_ID);
	
	// Add the variable for every program
	for ( u32 i = 1; i < _nbPgm; i++ )
	{
		// Get the program
		auto it = m_programs.find(_pgmNameList[i]);
		NEP_ASSERT(it != m_programs.end());

		// Add its ID to the corresponding program
		it->second.m_uniformVarIDs.push_back(uniform_ID);
	}
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

void ViewSpawner::useModelViewAndProjectionMatrices(GraphicsProgram::ProgramName _pgmName)
{
	// Add World matrix
	GraphicsProgram::UniformVarInput world_matrix(NEP_UNIVNAME_MV_MATRIX,
		GraphicsProgram::FLOAT,
		4,
		4,
		sizeof(m_worldMatrix),
		&m_worldMatrix);

	addUniformVariable(_pgmName, world_matrix);

	// Add a default projection
	Mat4 I;
	GraphicsProgram::UniformVarInput projection_matrix(NEP_UNIVNAME_PROJ_MATRIX,
		GraphicsProgram::FLOAT,
		4,
		4,
		16*sizeof(float),
		&I);

	addUniformVariable(_pgmName, projection_matrix);
}

//!!!!! Slow! Really slow to execute
// Idea - execute it first before calling create.
// Then users could call a refresh method 
// if parameters have been added since first call.
void ViewSpawner::movePgmParameters()
{
	NEP_PROFILING_CHRONO_INIT;
	NEP_PROFILING_CHRONO_START;
	
	// Add the graphics programs and corresponding attributes to the view
	for(auto& it : m_programs)
	{
		// Pass the parameters to the program
		GraphicsProgram* pgm = it.second.m_program;

		// Shader attributes
		for(const auto& custom_data : it.second.m_shaderAttributesCustomData)
		{
			NEP_ASSERT(m_shaderAttributes.find(custom_data.m_id) != m_shaderAttributes.end());

			// Get the attribute from the attribute table
			GraphicsProgram::ShaderAttribute& att	= m_shaderAttributes[custom_data.m_id];
			
			// Set the layout for the current program
			att.m_layout							= custom_data.m_layout;
			
			// Add the attribute to the program
			pgm->addShaderAttribute( att );
		}

		// Uniform variables
		for(const auto& uniform_var_ID : it.second.m_uniformVarIDs)
		{
			NEP_ASSERT(m_uniformVariables.find(uniform_var_ID) != m_uniformVariables.end());
			pgm->addUniformVariable(m_uniformVariables[uniform_var_ID]);
		}

		// Textures
		for (auto& it_texture : it.second.m_textures)
		{
			pgm->setTexture(it_texture.second);
		}

		// Rebuild the program including all its parameters
		pgm->build();

		// Flush program's parameters
		it.second.m_shaderAttributesCustomData.clear();
		it.second.m_uniformVarIDs.clear();
	}

	double t = NEP_PROFILING_CHRONO_STOP;
	NEP_LOG("movePgmParameters time %f ms", t);
}