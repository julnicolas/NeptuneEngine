#ifndef NEPTUNE_PLY_LOADER_H
#define NEPTUNE_PLY_LOADER_H

#include "System/Type/Integers.h"
#include <vector>
#include <fstream>
#include <GL/glew.h>

// For now this class just performs a loading from a PLY text-file

namespace Neptune
{
	class PLYLoader
	{
	public:
		enum PropertyType
		{
			POSITION,
			COLOR   ,
			COLOR_ALPHA,
			NORMAL  ,
			INDEX  ,
			PROPERTY_COUNT	// Must always be the last line of the enum
		};

		enum ValueType
		{
			UCHAR,
			USHORT,
			UINT,
			FLOAT,
			DOUBLE
		};

		struct Property
		{
			ValueType    m_valueType;
			PropertyType m_propType;
			size_t       m_typeSize;

			Property(PropertyType type, ValueType valType, size_t size) :
				m_typeSize(size), m_valueType(valType), m_propType(type) {}
		};

		class Element
		{
		public:
			typedef std::vector<Property>::iterator Element::iterator;

			Element(u64 linesNb) : m_dataLinesNb(linesNb) {}
			void addProperty(Property& prop)              { m_properties.push_back(prop); }
			u64  getDataLinesNb() const                   { return m_dataLinesNb;         }
			iterator begin()                              { return m_properties.begin();  }
			iterator end()                                { return m_properties.end();    }

		private:
			u64                   m_dataLinesNb;
			std::vector<Property> m_properties;
		};

		struct PropertyData
		{
			ValueType m_valueType;
			size_t    m_bufferSize;
			void*     m_buffer;
		};

		PLYLoader();
		~PLYLoader();
		void  load(const char* relativeFilePath);
		PropertyData& getPropertyBuffer(PropertyType prop) { return m_propertyBuffers[prop]; }
		u32 getNbverticesToRender() const                  { return m_nbverticesToRender;    }
		
		// Returns a GL type
		GLushort getIndicesType() const                    { return m_indicesType;   }

	private:
		// File parsing/processing operations
		bool processFileHeader(std::ifstream& file);
		bool processFileBody(std::ifstream& file);
		bool parseHeaderLine(char* fileLine);
		bool parseProperty(char* fileLine);
		bool parseBodyLines(std::ifstream& file, char* fileLine, const unsigned char lineSize);

		// Buffer management
		void allocateBuffers(); // Use m_propertyListReadOrder
		bool allocateBuffer(PropertyType prop, ValueType valType, size_t size);
		void fillBuffer(PropertyType prop, void* data, size_t size);

		// Attributes
		std::vector< Element >                m_propertyListReadOrder;
		s8                                    m_activePropertyListIndex;
		u32                                   m_nbverticesToRender;
		GLushort                              m_indicesType;
		PropertyData                          m_propertyBuffers[PROPERTY_COUNT];
	};
}

namespace std // Must be changed
{
	inline bool operator==(const Neptune::PLYLoader::Property& obj1, const Neptune::PLYLoader::Property& obj2)
	{
		return memcmp(&obj1, &obj2, sizeof(obj1)) == 0;
	}
}

#endif