#pragma once

#include "Graphics/Factories/ViewSpawner.h"

namespace Neptune
{
	class  VAOView;
	struct Color;

	class TriangleSpawner: public ViewSpawner
	{
	public:
		//
		// C O N S T R U C T O R S
		//

		TriangleSpawner(const char* _pgmName, GraphicsProgram* _pgm):
			ViewSpawner(_pgmName, _pgm){}


		//
		// D E F A U L T   G E N E R A T E D   M E T H O D S
		//

		virtual ~TriangleSpawner()											= default;
		TriangleSpawner(const TriangleSpawner&)								= delete;
		virtual TriangleSpawner& operator=(const TriangleSpawner&)			= delete;


		//
		// S P E C I A L I Z E D   M E T H O D S 
		//

		bool		CreateColorData(const Color& _c)		override;		/// Creates per-vertex color data, if the data were already created, the color will be changed (for all the view instantiated by the factory). The input color for this method is not referenced.
		bool		CreateNormalData()						override;		/// Creates the normals at every vertex of the view
		bool		Create2DTextureMapData()				override;		/// Creates the data to be able to map a 2D texture on the whole view
	
	protected:
		void		createVertexData()						override;
		View*		CreateViewAndSetUpRenderParameters()	override;

	private:
		void initData(const Color& color);
	};
}

