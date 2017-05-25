#pragma once

/// In this file are registered every common shader-uniform-variable-names
/// with a preprocessor constant to portably refer to them throughout the 
/// the engine's code.

/// Format - NEP_UNIVNAME_sensible_name_for_your_uniform_variable "Uniform variable's name in vertex shaders"


// Space-transformation matrices
#define NEP_UNIVNAME_MV_MATRIX     "ModelView"
#define NEP_UNIVNAME_PROJ_MATRIX   "Projection"

// Lights
#define NEP_UNIVNAME_AMBIENT_LIGHT "AmbientLight"