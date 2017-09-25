#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

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
	// Because of the perspective projection, the mv matrix is not orthogonal
	// Therefore, the ModelView matrix cannot be used as is to transform normal vectors (they can end up not orthogonal to their surface...)
	// That's why ModelView^-1 is used
	vec4 n = inverse(ModelView) * vec4(normal, 1.0); // If the player moves normal shouldn't be recomputed. They should be if the object rotate on itself.
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