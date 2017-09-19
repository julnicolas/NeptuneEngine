#pragma once

#include "Graphics/Spawners/ViewSpawner.h"

namespace Neptune
{
	class  VAOView;
	struct Color;

	class TriangleSpawner final: public ViewSpawner
	{
	public:
		//
		// C O N S T R U C T O R S
		//

		TriangleSpawner(GraphicsProgram* _pgm):
			ViewSpawner(_pgm){}


		//
		// D E F A U L T   G E N E R A T E D   M E T H O D S
		//

		virtual ~TriangleSpawner()											= default;
		TriangleSpawner(const TriangleSpawner&)								= delete;
		virtual TriangleSpawner& operator=(const TriangleSpawner&)			= delete;


		//
		// S P E C I A L I Z E D   M E T H O D S 
		//

		void		createVertexData()						override;
		bool		createColorData(const Color& _c)		override;		/// Creates per-vertex color data, if the data were already created, the color will be changed (for all the view instantiated by the factory). The input color for this method is not referenced.
		bool		createNormalData()						override;		/// Creates the normals at every vertex of the view
		bool		create2DTextureMapData()				override;		/// Creates the data to be able to map a 2D texture on the whole view
	
	protected:
		View*		createViewAndSetUpRenderParameters()	override;
	};
}

