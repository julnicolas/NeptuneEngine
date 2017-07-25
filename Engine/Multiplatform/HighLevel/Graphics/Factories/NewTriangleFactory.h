#include "Graphics/ViewFactory.h"
#include "Graphics/Color.h"
#include <vector>

namespace Neptune
{
	class  Shader;
	class  GraphicsProgram;

	class NewTriangleFactory : public ViewFactory
	{
	public:
		//! Add something to access the default program's name

		NewTriangleFactory(const Color& _c); // Use the default shaders first and GenerateColorData()
		NewTriangleFactory(Texture* _t);     // Use the default shaders first and Create2DTextureMapData()

		// Set the shader to use as a first graphics program
		// Program parameters have to be passed manually
		// As well as color data, normals or 2D-texture-map coordinates (call the factory's methods for these ones)
		NewTriangleFactory(const char* _pgmName, Shader* _shaderList, size_t _size);

		virtual bool GenerateColorData(const Color& _c);	// Creates per-vertex color data, if the data were already created, the color will be changed (for all the view instantiated by the factory)
		virtual bool CreateNormalData();					// Creates the normals at every vertex of the view
		virtual bool Create2DTextureMapData();				// Creates the data to be able to map a 2D texture on the view

		const std::vector<Color>& getColorData()        const;
		const std::vector<float>& getNormalData()       const;
		const std::vector<float>& get2DTextureMapData() const;

		GraphicsProgram& addGraphicsProgram(const char* _name); // Add another graphics program, this will add another draw call for the current view
		GraphicsProgram* getGraphicsProgram(const char* _name); // Returns nullptr if the program doesn't exist

		virtual View* create() override; // Allocates the view on the heap

	};
}