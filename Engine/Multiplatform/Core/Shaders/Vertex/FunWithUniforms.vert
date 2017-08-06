#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

uniform float Time;		//seconds

out vec3 v_color;

void main()
{
	int t		= int(Time);
	if ( t%2 == 0)
		v_color = color.rgb;
	else
		v_color = color.bgr;
	
	gl_Position = vec4( position, 1.0 );
}