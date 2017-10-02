#include "Graphics/PLYLoader.h"
#include "Debug/NeptuneDebug.h"
#include "Math/Math.h"
#include <fstream>
#include <algorithm>

using namespace Neptune;

const std::streamsize WORD_SIZE = 9;

const size_t UNKNOWN_TYPE = ~0;
static size_t GetTypeSizeFromHeaderLine(const char* type, PLYLoader::ValueType& valType)
{
	if ( !strcmp(type, "float") )
	{
		valType = PLYLoader::ValueType::FLOAT;
		return sizeof(float);
	}

	if ( !strcmp(type, "uchar") )
	{
		valType = PLYLoader::ValueType::UCHAR;
		return sizeof(unsigned char);
	}

	if ( !strcmp(type, "uint") )
	{
		valType = PLYLoader::ValueType::UINT;
		return sizeof(unsigned int);
	}

	if ( !strcmp(type, "ushort") )
	{
		valType = PLYLoader::ValueType::USHORT;
		return sizeof(unsigned short);
	}

	if ( !strcmp(type, "double") )
	{
		valType = PLYLoader::ValueType::DOUBLE;
		return sizeof(double);
	}

	return UNKNOWN_TYPE;
}

const GLushort WRONG_GL_TYPE = 0;
static GLushort GetGLTypeFromPLYLoaderValueType(PLYLoader::ValueType type)
{
	switch ( type )
	{
	case PLYLoader::FLOAT:
		return GL_FLOAT;

	case PLYLoader::UCHAR:
		return GL_UNSIGNED_BYTE;

	case PLYLoader::USHORT:
		return GL_UNSIGNED_SHORT;

	case PLYLoader::UINT:
		return GL_UNSIGNED_INT;

	case PLYLoader::DOUBLE:
		return GL_DOUBLE;

	default:
		return WRONG_GL_TYPE;
	}
}

PLYLoader::PLYLoader():
	m_nbverticesToRender(0), m_activePropertyListIndex(-1),
	m_indicesType(WRONG_GL_TYPE)
{
	memset(m_propertyBuffers, 0, sizeof(m_propertyBuffers));
}

PLYLoader::~PLYLoader()
{
	for ( u8 i = 0; i < PROPERTY_COUNT; i++ )
	{
		if ( m_propertyBuffers[i].m_buffer != NULL ) 
			free( m_propertyBuffers[i].m_buffer );
	}
}

void PLYLoader::load(const char* relativeFilePath)
{
	std::ifstream model_file( relativeFilePath );
	NEP_ASSERT( model_file.is_open() );

	bool success = processFileHeader( model_file );
	NEP_ASSERT( success );
	
	allocateBuffers();

	success = processFileBody( model_file );
	NEP_ASSERT( success );
}

static const size_t getBiggestSupportedTypeSize()
{
	return Max( (u8) sizeof(unsigned int), (u8) sizeof(float) );
}

bool PLYLoader::parseHeaderLine(char* fileLine)
{
	char* word = NULL;
	const char SEPARATION_CHAR[] = " ";
	word = strtok( fileLine, SEPARATION_CHAR );

	if ( !strcmp(word, "element") )
	{
		word = strtok(NULL, SEPARATION_CHAR); // get next word
		if (!strcmp(word, "vertex"))
		{
			word = strtok(NULL, SEPARATION_CHAR);
			sscanf( word, "%u", &m_nbverticesToRender);
			m_propertyListReadOrder.push_back( Element(m_nbverticesToRender) );
			m_activePropertyListIndex++;
			
			return true;
		}

		if (!strcmp(word, "face"))
		{
			word = strtok(NULL, SEPARATION_CHAR); // get next word
			u64 face_number = 0;
			sscanf(word, "%ull", &face_number);
			m_propertyListReadOrder.push_back( Element(face_number) );
			m_activePropertyListIndex++;

			return true;
		}

		NEP_ASSERT(m_propertyListReadOrder.size() != 0);

		return true; // Unknown element
	}

	if ( !strcmp(word, "property") )
	{
		parseProperty(word);
	}

	if (!strcmp(word, "end_header"))
		return false;

	return true; // Unknown word
}

bool PLYLoader::parseProperty(char* fileLine)
{
	char* word = NULL;
	const char SEPARATION_CHAR[] = " ";
	word = strtok(NULL, SEPARATION_CHAR); // Get the word following "property". It is assumed that strtok was previously called.
	
	Element& property_order_list = m_propertyListReadOrder[m_activePropertyListIndex];

	if ( strcmp(word, "list") ) // Property's type is standard
	{
		ValueType val_type;
		size_t type_size = GetTypeSizeFromHeaderLine(word, val_type);
		NEP_ASSERT(type_size != UNKNOWN_TYPE);

		word = strtok(NULL, SEPARATION_CHAR);
		if ( !strcmp(word, "x") ) // It is assumed that y and z will directly follow
		{
			property_order_list.addProperty(Property(POSITION, val_type, type_size));
			return true;
		}

		if ( !strcmp(word, "nx") ) // It is assumed that ny and nz will directly follow
		{
			property_order_list.addProperty(Property(NORMAL, val_type, type_size));
			return true;
		}

		if ( !strcmp(word, "red") ) // It is assumed that green and blue will directly follow
		{
			property_order_list.addProperty(Property(COLOR, val_type, type_size));
			return true;
		}

		if ( !strcmp(word, "alpha") ) // The color buffer must have an alpha-channel-field
		{
			Element::iterator it_end = property_order_list.end();
			Element::iterator it = std::find(property_order_list.begin(), it_end, Property(COLOR, val_type, type_size));
			
			if ( it != it_end )     // Replace the current RGB buffer by a RGBA
				*it = Property(COLOR_ALPHA, val_type, type_size);
			else 
				property_order_list.addProperty(Property(COLOR_ALPHA, val_type, type_size));

			return true;
		}

		return true; // Unknown property
	}
	else // Special process to support the "list" keyword
	{
		word = strtok(NULL, SEPARATION_CHAR); // We don't bother with the index-type of the list. An u8 is assumed to be enough.
		word = strtok(NULL, SEPARATION_CHAR); // Here the data type is gotten
		ValueType val_type;
		size_t type_size = GetTypeSizeFromHeaderLine( word, val_type );
		NEP_ASSERT( type_size != UNKNOWN_TYPE );

		word = strtok(NULL, SEPARATION_CHAR);
		if ( !strcmp(word, "vertex_indices") )
		{
			m_indicesType = GetGLTypeFromPLYLoaderValueType( val_type );
			NEP_ASSERT( m_indicesType != WRONG_GL_TYPE );
			property_order_list.addProperty(Property(INDEX, val_type, type_size));
			return true;
		}

		return true; // Unsupported data
	}
	

	return true; // Unknown property
}

bool PLYLoader::processFileHeader(std::ifstream& file)
{
	const unsigned char MAX_LINE_SIZE = 200;
	char  file_line[MAX_LINE_SIZE + 1] = "";
	
	bool success = false;
	do
	{
		file.getline( file_line, MAX_LINE_SIZE+1, '\n' );
		success = parseHeaderLine(file_line);
		
	} while ( success );


	return true;
}

bool PLYLoader::processFileBody(std::ifstream& file)
{
	const unsigned char MAX_LINE_SIZE = 200;
	char  file_line[MAX_LINE_SIZE + 1] = "";

	return parseBodyLines(file, file_line, MAX_LINE_SIZE);
}

static void convertStringToNumeral(const char* string_val, PLYLoader::ValueType type, void* o_value)
{
	switch ( type )
	{
	case PLYLoader::ValueType::UCHAR:
		*(unsigned char*)o_value = static_cast<unsigned char>( atoi(string_val) );
		break;

	case PLYLoader::ValueType::UINT:
		*(unsigned int*)o_value  = static_cast<unsigned int>( atoll(string_val) );
		break;

	case PLYLoader::ValueType::FLOAT:
		*(float*)o_value = static_cast<float>( atof(string_val) );
		break;

	case PLYLoader::ValueType::DOUBLE:
		*(double*)o_value = atof(string_val);
		break;

	default:
		NEP_ASSERT(false);
		break;
	}
}

static void convertNValues(char*& ascii_values, void* o_val, PLYLoader::ValueType type, size_t typeSize, u8 nb_val, size_t& lineIterator)
{
	const char SEPARATION_CHAR[] = " ";
	
	for ( u8 i = 0; i < nb_val; i++ )
	{
		convertStringToNumeral(ascii_values, type, (void*)((size_t)o_val + lineIterator));
		ascii_values = strtok(NULL, SEPARATION_CHAR); // Pb here. The same value is always read
		lineIterator += typeSize;
	}
}

static void readPropertyValues(void* data, const PLYLoader::Property& prop, char*& ascii_values, size_t& lineIterator)
{
	// Convert ascii values into numerals
	const char SEPARATION_CHAR[] = " ";

	switch ( prop.m_propType )
	{
	case PLYLoader::PropertyType::POSITION:
		convertNValues(ascii_values, data, prop.m_valueType, prop.m_typeSize, 3, lineIterator);
		break;

	case PLYLoader::PropertyType::COLOR:
		convertNValues(ascii_values, data, prop.m_valueType, prop.m_typeSize, 3, lineIterator);
		break;

	case PLYLoader::PropertyType::COLOR_ALPHA:
		convertNValues(ascii_values, data, prop.m_valueType, prop.m_typeSize, 4, lineIterator);
		break;

	case PLYLoader::PropertyType::NORMAL:
		convertNValues(ascii_values, data, prop.m_valueType, prop.m_typeSize, 3, lineIterator);
		break;

	case PLYLoader::PropertyType::INDEX:
	{
		unsigned char nb_val = 0;
		convertStringToNumeral(ascii_values, PLYLoader::ValueType::UCHAR, &nb_val);
		ascii_values = strtok(NULL, SEPARATION_CHAR); // Get the first value
		convertNValues(ascii_values, data, prop.m_valueType, prop.m_typeSize, nb_val, lineIterator);
	}
		break;

	default:
		NEP_ASSERT(false);
		break;
	}
}

bool Neptune::PLYLoader::parseBodyLines(std::ifstream& file, char* fileLine, const unsigned char lineSize)
{
	const char SEPARATION_CHAR[] = " ";
	void* data = malloc(getBiggestSupportedTypeSize() * 4); // Create a temp buffer to hold the values of a single line

	std::vector<Element>::iterator it_element_end = m_propertyListReadOrder.end();
	for ( std::vector<Element>::iterator it_element = m_propertyListReadOrder.begin(); it_element != it_element_end; ++it_element )
	{
		size_t line_count = 0;
		while ( line_count < it_element->getDataLinesNb() )
		{
			file.getline(fileLine, lineSize + 1, '\n');
			char* word = fileLine;
			strtok(word, SEPARATION_CHAR); // Get the first value

			for ( Element::iterator it_prop = it_element->begin(); it_prop != it_element->end(); ++it_prop )
			{
				size_t line_iterator = 0;
				readPropertyValues(data, *it_prop, word, line_iterator);

				// Copy the read data in the right buffer
				size_t data_size = ( it_prop->m_propType != COLOR_ALPHA ) ? 3 : 4;
				data_size       *= it_prop->m_typeSize;
				memcpy( (void*)((size_t)m_propertyBuffers[it_prop->m_propType].m_buffer + line_count*data_size), data, data_size );
			}

			line_count++;
		}
	}

	free( data );

	return true;
}

void PLYLoader::allocateBuffers()
{
	bool success = false;

	std::vector< Element >::iterator it_element_end = m_propertyListReadOrder.end();
	for ( std::vector< Element >::iterator it_element = m_propertyListReadOrder.begin(); it_element != it_element_end; ++it_element )
	{
		Element::iterator it_property_end = it_element->end();
		for ( Element::iterator it_property = it_element->begin(); it_property != it_property_end; ++it_property )
		{
			size_t buffer_size = 0;
			buffer_size     = ( it_property->m_propType != COLOR_ALPHA ) ? 3*it_property->m_typeSize : 4*it_property->m_typeSize; // Horrendous! Each and every property should be able to retrieve its size!
			buffer_size     *= it_element->getDataLinesNb();
			
			success = allocateBuffer( it_property->m_propType, it_property->m_valueType, buffer_size );
			NEP_ASSERT( success );
		}
	}
}

bool PLYLoader::allocateBuffer(PropertyType propType, ValueType valueType, size_t size)
{
	PropertyData data;
	data.m_valueType  = valueType;
	data.m_bufferSize = size;
	data.m_buffer     = malloc(size);

	m_propertyBuffers[propType] = data;

	return data.m_buffer != NULL;
}