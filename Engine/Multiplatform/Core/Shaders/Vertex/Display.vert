#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 v_color;

uniform mat4 ModelView;
uniform mat4 Projection;

void main()
{
	v_color = color;
	gl_Position = Projection * ModelView * vec4( position, 1.0 );
}