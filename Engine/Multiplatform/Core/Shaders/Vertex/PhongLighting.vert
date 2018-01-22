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
const vec3 SpecularLightColor = vec3(1.0, 1.0, 1.0);

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

out vec4		v_nornalWorldSpace;
out vec4		v_normalCameraSpace;
out vec4		v_eyePosCameraSpace;
out vec4		v_lightDirectionCameraSpace;

out vec4		v_ambientLghtColor;
out vec4		v_specularLightColor;


//////////////////////// LIGHT WORK

void PreparePhongLight()
{
	v_ambientLghtColor			= vec4(AmbientLightColor, 1.0);
	v_specularLightColor		= vec4(SpecularLightColor, 1.0);
	
	v_nornalWorldSpace			= World * vec4(normal, 0.0);
	v_normalCameraSpace			= normalize(transpose(ModelView) * vec4(normal, 0.0));
	v_eyePosCameraSpace			= normalize(vec4(0.0, 0.0, 0.0, 0.0) - ModelView * vec4(position, 0.0));
	v_lightDirectionCameraSpace	= normalize(ModelView * inverse(World) * vec4(DiffuseLightDirection, 0.0));
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
	PreparePhongLight();
	
	// Texture binding
	v_activeTextureBinding	= getCurrentTextureBinding();
	v_text					= text;
	
	// Vertex transformations
	gl_Position				= Projection * ModelView * vec4( position, 1.0 );
}