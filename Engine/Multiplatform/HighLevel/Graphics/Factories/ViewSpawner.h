#pragma once

#include "Graphics/GraphicsProgram.h"
#include "Graphics/Color.h"

#include <vector>
#include <unordered_map>

namespace Neptune
{
	class Shader;
	class Texture;
	class View;

	class ViewSpawner
	{
	public:
		//
		// C O N S T R U C T O R S
		//

		ViewSpawner(const Color& _c); // Use the default shaders first and GenerateColorData()
		ViewSpawner(Texture* _t);     // Use the default shaders first and Create2DTextureMapData()
		
		// Set the shader to use as a first graphics program
		// Program parameters have to be passed manually
		// As well as color data, normals or 2D-texture-map coordinates (call the factory's methods for these ones)
		ViewSpawner(const char* _pgmName, Shader* _shaderList, size_t _size);


		//
		// D E F A U L T   G E N E R A T E D   M E T H O D S
		//

		virtual ~ViewSpawner()						= default;
		ViewSpawner(const ViewSpawner&)				= delete;
		ViewSpawner& operator=(const ViewSpawner&)	= delete;


		//
		// P U R E   V I R T U A L   M E T H O D S 
		//

		virtual View* create() =0;													// Allocates the view on the heap


		//
		//
		//

		virtual bool GenerateColorData(const Color& _c);							// Creates per-vertex color data, if the data were already created, the color will be changed (for all the view instantiated by the factory)
		virtual bool CreateNormalData();											// Creates the normals at every vertex of the view
		virtual bool Create2DTextureMapData();										// Creates the data to be able to map a 2D texture on the view


		//
		//
		//

		const std::vector<Color>& getColorData()        const;						// You must create/generate these data first
		const std::vector<float>& getNormalData()       const;						// You must create/generate these data first
		const std::vector<float>& get2DTextureMapData() const;						// You must create/generate these data first


		//
		//
		//

		GraphicsProgram& addGraphicsProgram(const char* _name);						// Add another graphics program, this will add another draw call for the current view
		GraphicsProgram* getGraphicsPrograms(const char* _name, size_t& _size);		// Returns an array of GraphicsProgram (nullptr if none is present) and its size in _size (0 if no program).


	protected:
		struct Program // Maybe not necessary
		{
			GraphicsProgram									m_program;
			std::vector<GraphicsProgram::ShaderAttribute>	m_shaderAttributes;
			std::vector<GraphicsProgram::UniformVarInput>	m_uniformVars;
			// Do I add a std::vector<void*> for all the shader attributes data? 
		};

		std::vector<float>					m_vertices;
		std::vector<Color>					m_colors;
		std::vector<float>					m_texCoords;
		std::vector<float>					m_normals;
		std::unordered_map<char*, Program>	m_programs; // Need to manually set the hash function
		Texture*                            m_texture; // Texture to be used in the default shaders. Probably will be refactored later
	};
}