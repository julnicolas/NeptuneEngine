#version 430 core

// Inputs
in vec2									v_text;
flat in uint							v_activeTextureBinding;
in vec4									v_ambientLghtColor;
in vec4									v_diffuseLightIntensity;
in vec4									v_specularLightIntensity;


// Uniforms / Constants
const uint								NB_SUPPORTED_SAMPLERS = 16;
layout (binding = 0) uniform sampler2D	Texture[NB_SUPPORTED_SAMPLERS];

// Outputs
layout (location = 0) out vec4 			f_color;

// Debug display
vec4 Colors[NB_SUPPORTED_SAMPLERS] = {
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
};

void main()
{
	vec4 diffuseMatColor  = texture(Texture[v_activeTextureBinding % NB_SUPPORTED_SAMPLERS], v_text);
	vec4 specularMatColor = vec4(1.0, 1.0, 1.0, 1.0);
	
	vec4 color = min(v_ambientLghtColor * diffuseMatColor + v_diffuseLightIntensity * diffuseMatColor + v_specularLightIntensity * specularMatColor, vec4(1.0, 1.0, 1.0, 1.0));
	
	// If fragment is transparent, discard it
	if ( color.a == 0.0f )
		discard;
	
	f_color = color.rgba;
}