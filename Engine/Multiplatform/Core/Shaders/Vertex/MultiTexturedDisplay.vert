#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 text;

out vec2	v_text;
flat out int	v_activeTextureBinding;

uniform mat4 ModelView;
uniform mat4 Projection;

// This shader supports up to 16 textures bound at the same time.
// The array contains the index of every last vertex using a texture.
// Let's say 2 textures are used, then the array will look like:
// TextureBindingIndexTable[0] = 10;
// TextureBindingIndexTable[1] = last_vertex;
// Therefore, from vertex 0 to 10 the first texture must be used.
// From 11 to last_vertex, it must be the second.
uniform int TextureBindingIndexArray[26]; // i: vertex id, i+1: texture binding value

int getCurrentTextureBinding()
{
	for (int i = 0; i < 26; i+=2)
	{
		if (gl_VertexID <= TextureBindingIndexArray[i])
			return TextureBindingIndexArray[i+1];
	}
	
	return 0; // error, the index array is misconfigured
}

void main()
{
	v_activeTextureBinding = getCurrentTextureBinding();
	v_text = text;
	gl_Position = Projection * ModelView * vec4( position, 1.0 );
}