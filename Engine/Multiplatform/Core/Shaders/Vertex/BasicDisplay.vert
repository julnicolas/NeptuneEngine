#version 430 core

layout (location = 0) in vec3 positions;
layout (location = 1) in vec2 text;

out vec4 v_color;
out vec2 v_text;

uniform mat4 ModelView;

void main()
{
	v_text = text;
	gl_Position = ModelView*vec4( positions.xyz, 1.0 );
}