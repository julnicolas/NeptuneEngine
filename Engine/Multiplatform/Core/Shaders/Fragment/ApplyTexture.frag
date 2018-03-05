#version 430 core

// Inputs / Outputs
in vec2									v_text;
flat in uint							v_activeTextureBinding;
layout (location = 0) out vec4 			f_color;

// Uniforms / Constants
const uint								NB_SUPPORTED_SAMPLERS = 32;
layout (binding = 0) uniform sampler2D	Texture[NB_SUPPORTED_SAMPLERS];

// Debug display
/*vec4 Colors[NB_SUPPORTED_SAMPLERS] = {
	vec4(1.0, 0.0, 0.0,1.0),
	vec4(1.0, 1.0, 0.0,1.0),
	vec4(1.0, 1.0, 1.0,1.0),
	vec4(0.0, 1.0, 0.0,1.0),
	
	vec4(0.0, 1.0, 1.0,1.0),
	vec4(1.0, 0.75, 0.0,1.0),
	vec4(1.0, 0.0, 0.75,1.0),
	vec4(0.0, 1.0, 0.75,1.0),
	
	vec4(0.5, 1.0, 0.0,1.0),
	vec4(0.5, 0.0, 1.0,1.0),
	vec4(0.0, 0.5, 1.0,1.0),
	vec4(1.0, 0.5, 0.25,1.0),
	
	vec4(1.0, 0.25, 0.5,1.0),
	vec4(0.25, 1.0, 0.5,1.0),
	vec4(0.5, 0.25, 1.0,1.0),
	vec4(0.5, 1.0, 0.25,1.0)
};*/

void main()
{
	vec4 text_col = texture(Texture[v_activeTextureBinding % NB_SUPPORTED_SAMPLERS], v_text);
	
	// If fragment is transparent, discard it
	if ( text_col.a == 0.0f )
		discard;
	
	f_color = text_col.rgba;
}