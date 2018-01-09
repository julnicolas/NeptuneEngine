#pragma once

/// In this file are registered every common shader-uniform-variable-names
/// with a preprocessor constant to portably refer to them throughout 
/// the engine's code.

/// Format - NEP_UNIVNAME_sensible_name_for_your_uniform_variable "Uniform variable's name in vertex shaders"


// Space-transformation matrices
#define NEP_UNIVNAME_MV_MATRIX						"ModelView"
#define NEP_UNIVNAME_PROJ_MATRIX					"Projection"

// Lights
#define NEP_UNIVNAME_AMBIENT_LIGHT					"AmbientLight"
#define NEP_UNIVNAME_DIFFUSE_LIGHT_DIRECTION		"DiffuseLightDirection"
#define NEP_UNIVNAME_DIFFUSE_LIGHT_COLOR			"DiffuseLightColor"

// Multi-texturing

/// \brief	This variable is a set of pairs of the following
///			<last_vertex_index, texture_binding_point>.
///			Let's say one wants to texture two plans with
///			2 different textures of binding points 0 and 1.
///			The array will be {5, 0, 11, 1} with the pair
///			<5, 0> meaning - from vertex 0 to 5 use texture
///			at binding point 0. Then pair <11, 1> meaning -
///			from vertices 6 to 11 use texture at binding
///			point 1.
#define NEP_UNIVNAME_VERTICES_TO_TEXTURE_BINDING_POINT_MAP	"VerticesToTextureBindingPointMap"