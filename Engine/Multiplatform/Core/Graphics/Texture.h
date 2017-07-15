#pragma once

#include "StdInterface/Updatable.h"
#include "System/Type/Integers.h"

namespace Neptune
{
	class Texture final: public Updatable
	{
	public:
		Texture(const char* name);
		~Texture();
		Texture(const Texture& t)             = delete;
		Texture& operator=(const Texture& t)  = delete;

		bool init()      final override;
		bool update()    final override;
		void terminate() final override;

		s32         getWidth()          const { return m_width;              }
		s32         getHeight()         const { return m_height;             }
		s32         getNbCompPerPixel() const { return m_componentsPerPixel; }
		const char* getName()           const { return m_name;               }
		bool        isInitialized()     const { return m_textureID != 0;     }

	private:
		void CreateTexture(u8* data, s32 reqComp);
		void SetPlaceHolderTexture(u8*& data);

		u32   m_textureID;
		s32   m_width;
		s32   m_height;
		s32   m_componentsPerPixel;
		char* m_name;
	};
}