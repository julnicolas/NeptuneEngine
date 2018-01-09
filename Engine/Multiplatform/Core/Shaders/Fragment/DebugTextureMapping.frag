#version 430 core

// Inputs / Outputs
in vec2							v_text;
flat in int						v_activeTextureBinding;
layout (location = 0) out vec4 		f_color;

// Uniforms / Constants
const int							NB_SUPPORTED_SAMPLERS = 4;
layout (binding = 0) uniform sampler2D Texture[NB_SUPPORTED_SAMPLERS];

vec4 getColor()
{
	switch (v_activeTextureBinding)
	{
		case 0:
			return vec4(1,0,0,1); // red
			
		case 1:
			return vec4(0,1,0,1); // green
			
		case 2:
			return vec4(0,0,1,1); // blue
			
		case 3:
			return vec4(1,1,0,1); // yellow
			
		default: // error
			return vec4(0,1,1,1); // cyan
	}
}

void main()
{
	f_color = getColor();
}