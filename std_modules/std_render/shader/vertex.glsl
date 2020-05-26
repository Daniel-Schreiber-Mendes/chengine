#version 300 es

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec3 v_texCoord; //v_ prefix stands for variing. these variables are passed between shaders

uniform vec2 u_texture_offsets[16];
uniform vec2 u_texture_sizes[16];
uniform uint u_texture_layers[16];
uniform mat4 u_transforms[16];

layout (std140) uniform WorldBlock
{
					//base alignment //aligned offset
	mat4 u_camera_vp; //64              0
};


void main()
{
	gl_Position = u_camera_vp * u_transforms[gl_InstanceID] * vec4(position, 1);
	v_texCoord = vec3(texCoord * u_texture_sizes[gl_InstanceID] + u_texture_offsets[gl_InstanceID], float(u_texture_layers[gl_InstanceID]));
}