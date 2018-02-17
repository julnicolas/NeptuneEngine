#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

uniform mat4 World;
uniform mat4 ModelView;
uniform mat4 Projection;

uniform vec3 DiffuseLightDirection; // Left-handed frame. Frame is the world frame
uniform vec3 DiffuseLightColor;

const vec3 AmbientLightColor = vec3(0.2, 0.2, 0.2);
const vec3 SpecularLightColor = vec3(1.0, 1.0, 1.0);

out vec4 v_color;

vec3 BlendColors(vec3 light, vec3 obj)
{
	return vec3(light.r*obj.r, light.g*obj.g, light.b*obj.b);
}

vec3 ComputeAmbientLight()
{
	return BlendColors(AmbientLightColor.rgb, color);
}

vec3 ComputeDiffuseLight()
{
	vec4 n = World * vec4(normal, 0.0);
	float reflection_coef = max(dot(normalize(n.xyz), normalize(-DiffuseLightDirection.xyz)), 0);
	
	return reflection_coef * BlendColors(DiffuseLightColor, color);
}

vec3 ComputeSpecularLight()
{
	vec4 n						= normalize(transpose(ModelView) * vec4(normal, 0.0));
	vec4 eye_pos_camera_space	= normalize(vec4(0.0, 0.0, 0.0, 0.0) - ModelView * vec4(position, 0.0));
	vec4 light_dir_camera_space	= normalize(ModelView * inverse(World) * vec4(DiffuseLightDirection, 0.0));		// View * DiffuseLightDirection
	vec4 reflection				= reflect(-light_dir_camera_space, n);
	
	float cos_alpha = max(dot(eye_pos_camera_space, reflection), 0.0);
	float shininess = 15;
	
	return SpecularLightColor * pow(cos_alpha, shininess);
}

vec3 ComputePhongLight()
{	
	return min(ComputeAmbientLight() + ComputeDiffuseLight() + ComputeSpecularLight(), vec3(1.0, 1.0, 1.0));
}

void main()
{
	// Compute diffuse light
	v_color = vec4(ComputePhongLight(), 1.0);

	// Transition to eye frame then Perspective projection
	gl_Position = Projection * ModelView * vec4( position, 1.0 );
}