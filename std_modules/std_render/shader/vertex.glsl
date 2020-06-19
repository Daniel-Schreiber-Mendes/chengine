#version 430

//per vertex
in vec3 position;
in vec2 texCoord;
//per instance
in vec2 textureOffset;
in vec2 textureSize;
in vec4 r1, r2, r3, r4; //mat4 splitted into 4 vec4'stransform

out vec3 v_texCoord;

uniform uint u_texture_layers[16];

layout (std140, binding = 0) uniform Camera
{
					//base alignment //aligned offset
	mat4 u_camera_vp; //64              0
};



void main()
{
	gl_Position = u_camera_vp * mat4(r1, r2, r3, r4) * vec4(position, 1);
	v_texCoord = vec3(texCoord * textureSize + textureOffset, float(u_texture_layers[gl_InstanceID]));
}