#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

uniform mat4 World;
uniform mat4 ModelView;
uniform mat4 Projection;

uniform vec3 DiffuseLightDirection; // Left-handed frame. Frame is the world frame
uniform vec3 DiffuseLightColor;

out vec4 v_color;

vec3 BlendColors(vec3 light, vec3 obj)
{
	return vec3(light.r*obj.r, light.g*obj.g, light.b*obj.b);
}

vec3 ComputeDiffuseLight()
{
	vec4 n = World * vec4(normal, 0.0);
	float reflection_coef = max(dot(normalize(n.xyz), normalize(-DiffuseLightDirection.xyz)), 0);
	
	return reflection_coef * BlendColors(DiffuseLightColor, color);
}

void main()
{
	// Compute diffuse light
	v_color = vec4(ComputeDiffuseLight(), 1.0);

	// Transition to eye frame then Perspective projection
	gl_Position = Projection * ModelView * vec4( position, 1.0 );
}