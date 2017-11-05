#version 430 core

// Inputs / Outputs
in vec2							v_text;
flat in int						v_activeTextureBinding;
layout (location = 0) out vec4 		f_color;

// Uniforms / Constants
const int							NB_SUPPORTED_SAMPLERS = 16;
layout (binding = 0) uniform sampler2D Texture[NB_SUPPORTED_SAMPLERS];

// Program

sampler2D getSampler()
{
	if (v_activeTextureBinding < NB_SUPPORTED_SAMPLERS)
		return Texture[v_activeTextureBinding];
	else // Error
		return Texture[0];
}

void main()
{
	vec4 text_col = texture(getSampler(), v_text);
	
	// If fragment is transparent, discard it
	if ( text_col.a == 0.0f )
		discard;
	
	f_color = text_col.rgba;
}