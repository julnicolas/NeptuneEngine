#version 430 core

uniform sampler2D Texture;

in vec2 v_text;

layout (location = 0) out vec4 f_color;

void main()
{
	vec4 text_col = texture(Texture, v_text);
	f_color = text_col.rgba;
}