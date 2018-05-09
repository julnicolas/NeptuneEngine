#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;

uniform mat4 World;
uniform mat4 ModelView;
uniform mat4 Projection;

uniform vec3 DiffuseLightDirection; // Left-handed frame. Frame is the world frame
uniform vec3 DiffuseLightColor;

const vec3 AmbientLightColor = vec3(0.2, 0.2, 0.2);
const vec3 SpecularLightColor = vec3(1.0, 1.0, 1.0);

/////// OUTPUT

out vec4		v_color;
out vec4		v_nornalWorldSpace;
out vec4		v_normalCameraSpace;
out vec4		v_eyePosCameraSpace;
out vec4		v_lightDirectionCameraSpace;

out vec4		v_ambientLghtColor;
out vec4		v_specularLightColor;


//////////////////////// LIGHT WORK

void PreparePhongLight()
{
	v_ambientLghtColor			= vec4(AmbientLightColor, 1.0);
	v_specularLightColor		= vec4(SpecularLightColor, 1.0);
	
	v_nornalWorldSpace			= World * vec4(normal, 0.0);
	v_normalCameraSpace			= normalize(transpose(ModelView) * vec4(normal, 0.0));
	v_eyePosCameraSpace			= normalize(vec4(0.0, 0.0, 0.0, 0.0) - ModelView * vec4(position, 0.0));
	v_lightDirectionCameraSpace	= normalize(ModelView * inverse(World) * vec4(DiffuseLightDirection, 0.0));
}

void main()
{
	PreparePhongLight();
	v_color = color;
	
	// Vertex transformations
	gl_Position				= Projection * ModelView * vec4( position, 1.0 );
}