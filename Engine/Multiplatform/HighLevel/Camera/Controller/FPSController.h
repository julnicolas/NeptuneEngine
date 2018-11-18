#pragma once
#include "Input/InputTypes.h"

namespace Neptune
{
	class Camera;

	class FPSController
	{
	public:
		FPSController(Camera& _camera);

		/// Move function. Implements InputConsumer's callback interface. 
		/// It moves a camera in a FPS fashion. The controls are WASD for forward, left, backward and right.
		/// It is possible to accelerate and decelerate using respectively Z or X.
		/// A qwerty keyboard has been considered to enumerate the above keys. On any other keyboards, different
		/// key values are used but the key locations to be considered are the same as the qwerty's. For instance,
		/// Z will be forward on an azerty keyboard instead of W on a qwerty. The reason is that the Z key substitutes
		/// the W on such devices.
		/// \param _input Input provided by a producer. Dependent on the input library used for the platform
		/// \param _camera Camera to be moved regarding the provided input
		/// \param _outStepDistance Output variable telling how far a step goes. The value is modified when pressing
		/// X (acceleration) or Z (deceleration).
		void operator()(const Input& _input);

	private:
		float	m_stepDistance;		/// Value determining the distance covered by a step.
		Camera&	m_camera;			/// Camera to be moved regarding the provided input
	};
}