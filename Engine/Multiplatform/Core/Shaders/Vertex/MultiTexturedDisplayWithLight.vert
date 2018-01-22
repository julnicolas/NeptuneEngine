#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 text;
layout (location = 2) in vec3 normal;

uniform mat4 World;
uniform mat4 ModelView;
uniform mat4 Projection;

uniform vec3 DiffuseLightDirection; // Left-handed frame. Frame is the world frame
uniform vec3 DiffuseLightColor;

const vec3 AmbientLightColor = vec3(0.2, 0.2, 0.2);
const vec3 SpecularLightColor = vec3(1.0, 1.0, 1.0);//vec3(1.0, 1.0, 1.0);

// This shader supports up to 16 textures bound at the same time.
// The array contains the index of every last vertex using a texture.
// Let's say 2 textures are used, then the array will look like:
// TextureBindingIndexTable[0] = 10;
// TextureBindingIndexTable[1] = last_vertex;
// Therefore, from vertex 0 to 10 the first texture must be used.
// From 11 to last_vertex, it must be the second.
uniform uint VerticesToTextureBindingPointMap[32]; // i: vertex id, i+1: texture binding value

/////// OUTPUT
out vec2		v_text;
flat out uint	v_activeTextureBinding;

out vec4		v_ambientLghtColor;
out vec4		v_diffuseLightIntensity;
out vec4		v_specularLightIntensity;


//////////////////////// LIGHT WORK

vec4 ComputeDiffuseLight()
{
	vec4 n = World * vec4(normal, 0.0);
	float reflection_coef = max(dot(normalize(n.xyz), normalize(-DiffuseLightDirection.xyz)), 0);
	
	return reflection_coef * vec4(DiffuseLightColor, 1.0);
}

vec4 ComputeSpecularLight()
{
	vec4 n						= normalize(transpose(ModelView) * vec4(normal, 0.0));
	vec4 eye_pos_camera_space	= normalize(vec4(0.0, 0.0, 0.0, 0.0) - ModelView * vec4(position, 0.0));
	vec4 light_dir_camera_space	= normalize(ModelView * inverse(World) * vec4(DiffuseLightDirection, 0.0));		// View * DiffuseLightDirection
	vec4 reflection				= reflect(-light_dir_camera_space, n);
	
	float cos_alpha = max(dot(eye_pos_camera_space, reflection), 0.0);
	float shininess = 5;
	
	return vec4(SpecularLightColor, 1.0) * pow(cos_alpha, shininess);
}

void ComputePhongLight()
{
	v_ambientLghtColor			= vec4(AmbientLightColor, 1.0);
	v_diffuseLightIntensity		= min(ComputeDiffuseLight(), vec4(1.0, 1.0, 1.0, 1.0));
	v_specularLightIntensity	= min(ComputeSpecularLight(), vec4(1.0, 1.0, 1.0, 1.0));
}

//////////////////////// TEXTURE WORK
uint getCurrentTextureBinding()
{
	for (uint i = 0; i < 32; i+=2)
	{
		if (gl_VertexID <= VerticesToTextureBindingPointMap[i])
			return VerticesToTextureBindingPointMap[i+1];
	}
	
	return 0; // error, the index array is misconfigured
}

void main()
{
	ComputePhongLight();
	
	v_activeTextureBinding = getCurrentTextureBinding();
	v_text = text;
	gl_Position = Projection * ModelView * vec4( position, 1.0 );
}