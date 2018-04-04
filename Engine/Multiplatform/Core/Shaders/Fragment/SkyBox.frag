#version 430 core
 
in vec3		v_cubeTexCoord;
out vec4	f_fragColor;

uniform samplerCube Cubemap;
 
void main (void)
{
    f_fragColor = texture(Cubemap, v_cubeTexCoord);
}