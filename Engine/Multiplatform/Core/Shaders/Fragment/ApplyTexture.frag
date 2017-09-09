#version 430 core

layout (binding = 0) uniform sampler2D Texture;

in vec2 v_text;

layout (location = 0) out vec4 f_color;

void main()
{
	vec4 text_col = texture(Texture, v_text);
	
	// If fragment is transparent, discard it
	if ( text_col.a == 0.0f )
		discard;
	
	f_color = text_col.rgba;
}