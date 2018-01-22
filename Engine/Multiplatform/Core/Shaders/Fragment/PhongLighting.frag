#version 430 core

// Inputs
in vec2		v_text;
flat in uint	v_activeTextureBinding;

in vec4		v_nornalWorldSpace;
in vec4		v_normalCameraSpace;
in vec4		v_eyePosCameraSpace;
in vec4		v_lightDirectionCameraSpace;

in vec4		v_ambientLghtColor;
in vec4		v_specularLightColor;


// Uniforms / Constants
const uint								NB_SUPPORTED_SAMPLERS = 16;
layout (binding = 0) uniform sampler2D	Texture[NB_SUPPORTED_SAMPLERS];

uniform vec3 DiffuseLightDirection; // Left-handed frame. Frame is the world frame
uniform vec3 DiffuseLightColor;

// Outputs
layout (location = 0) out vec4 			f_color;

//////////////////////// LIGHT WORK

vec4 ComputeDiffuseLight()
{
	float reflection_coef = max(dot(normalize(v_nornalWorldSpace.xyz), normalize(-DiffuseLightDirection.xyz)), 0);
	
	return reflection_coef * vec4(DiffuseLightColor, 1.0);
}

vec4 ComputeSpecularLight()
{
	vec4 reflection	= reflect(-v_lightDirectionCameraSpace, v_normalCameraSpace);
	float cos_alpha	= max(dot(v_eyePosCameraSpace, reflection), 0.0);
	
	float shininess = 5;
	
	return v_specularLightColor * pow(cos_alpha, shininess);
}

vec4 ComputePhongLight(vec4 _diffuseMatColor)
{
	vec4 diffuseLightIntensity	= min(ComputeDiffuseLight(), vec4(1.0, 1.0, 1.0, 1.0));
	vec4 specularLightIntensity	= min(ComputeSpecularLight(), vec4(1.0, 1.0, 1.0, 1.0));
	
	vec4 specularMatColor = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 color = min(v_ambientLghtColor * _diffuseMatColor + diffuseLightIntensity * _diffuseMatColor + specularLightIntensity * specularMatColor, vec4(1.0, 1.0, 1.0, 1.0));
	
	return color;
}

void main()
{
	vec4 diffuseMatColor  	= texture(Texture[v_activeTextureBinding % NB_SUPPORTED_SAMPLERS], v_text);
	vec4 color				= ComputePhongLight(diffuseMatColor);
	
	// If fragment is transparent, discard it
	if ( color.a == 0.0f )
		discard;
	
	f_color = color.rgba;
}