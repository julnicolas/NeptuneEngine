#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec3 v_color;

void main()
{
	v_color = color.rgb;
	gl_Position = vec4( position, 1.0 );
}