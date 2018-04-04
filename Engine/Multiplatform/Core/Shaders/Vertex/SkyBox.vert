#version 430 core

layout (location = 0) in vec3 position;
out vec3 v_cubeTexCoord;

uniform mat4 ModelView;
uniform mat4 Projection;

void main()
{
	v_cubeTexCoord = position;
	gl_Position = Projection * ModelView * vec4( position, 1.0 );
}