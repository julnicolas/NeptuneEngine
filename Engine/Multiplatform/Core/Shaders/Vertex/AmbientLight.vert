#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 v_color;

// Space-transformation matrices
uniform mat4 ModelView;
uniform mat4 Projection;

// Light sources
uniform vec3 AmbientLight;

void main()
{
	v_color = color * AmbientLight;
	gl_Position = Projection * ModelView * vec4( position, 1.0 );
}