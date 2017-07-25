#include "Graphics/Factories/NewTriangleFactory.h"
#include "Graphics/Shader.h"
#include "Graphics/GraphicsProgram.h"

using namespace Neptune;

// Statics

static const char S_DEFAULT_GRAPHICS_PROGRAM_NAME[] = "";

//

NewTriangleFactory::NewTriangleFactory(const Color& _c)
{

}

NewTriangleFactory::NewTriangleFactory(Texture* _t)
{

}

NewTriangleFactory::NewTriangleFactory(const char* _pgmName, Shader* _shaderList, size_t _size)
{

}

bool NewTriangleFactory::GenerateColorData(const Color& _c)
{

}

bool NewTriangleFactory::CreateNormalData()
{

}

bool NewTriangleFactory::Create2DTextureMapData()
{

}

const std::vector<Color>& NewTriangleFactory::getColorData() const
{
	return m_colors;
}

const std::vector<float>& NewTriangleFactory::getNormalData() const
{
	return m_normals; // must be added to ViewFactory
}

const std::vector<float>& NewTriangleFactory::get2DTextureMapData() const
{
	return m_texCoords;
}

GraphicsProgram& NewTriangleFactory::addGraphicsProgram(const char* _name)
{

}

GraphicsProgram* NewTriangleFactory::getGraphicsPrograms(const char* _name,size_t& _size)
{

}

View* NewTriangleFactory::create()
{

}