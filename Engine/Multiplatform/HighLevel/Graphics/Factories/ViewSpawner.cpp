#include "Graphics/Factories/ViewSpawner.h"
#include "Graphics/View.h"
#include "Graphics/Texture.h"
#include "System/Hashing/FastHashFunctions.h" // Must find out how to set unorderd_map hash function manually
#include "Debug/NeptuneDebug.h"

using namespace Neptune;

ViewSpawner::ViewSpawner(const char* _pgmName, GraphicsProgram* _pgm)
{
	addGraphicsProgram(_pgmName, _pgm);
}

void ViewSpawner::addGraphicsProgram(const char* _name, GraphicsProgram* _pgm)
{
	NEP_ASSERT(_name != nullptr);
	
	// Create a Program struct
	Program program;
	program.m_program = _pgm;

	// Add it to the program list if not present, otherwise do nothing
	size_t length = strlen(_name);
	m_programs.insert({ (const char*) Fnv1a32( (u8*) _name, length ), program});
}

void ViewSpawner::addShaderAttribute(const char* _pgmName, const GraphicsProgram::ShaderAttribute& _shaderAtt)
{
	NEP_ASSERT(_pgmName != nullptr);
	
	// Get the program
	const char* key = (const char*) Fnv1a32( (u8*) _pgmName, strlen(_pgmName) );
	auto it = m_programs.find(key);
	NEP_ASSERT( it != m_programs.end() );

	// Add the attribute if not already present
	const void* attribute_ID = _shaderAtt.m_data;
	m_shaderAttributes.insert({attribute_ID,_shaderAtt});

	// Add its ID to the corresponding program
	it->second.m_shaderAttributeIDs.push_back(attribute_ID);
}

void ViewSpawner::addUniformVariable(const char* _pgmName, const GraphicsProgram::UniformVarInput& _uniform)
{
	NEP_ASSERT(_pgmName != nullptr);

	// Get the program
	const char* key = (const char*)Fnv1a32((u8*)_pgmName,strlen(_pgmName));
	auto it = m_programs.find(key);
	NEP_ASSERT(it != m_programs.end());

	// Add the attribute if not already present
	const void* uniform_ID = _uniform.getData();
	m_uniformVariables.insert({uniform_ID,_uniform});

	// Add its ID to the corresponding program
	it->second.m_uniformVarIDs.push_back(uniform_ID);
}

bool ViewSpawner::MapColorData(const char* _pgmName, u8 _layout)
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

bool ViewSpawner::MapNormalData(const char* _pgmName, u8 _layout)
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

bool ViewSpawner::Map2DTextureMapData(const char* _pgmName, u8 _layout)
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
