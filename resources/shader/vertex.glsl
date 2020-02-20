#version 300 es

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord; //v_ prefix stands for variing. these variables are passed between shaders
out vec3 v_position;

uniform mat4 u_vpm; //u_ prefix stands for uniform. these variables are set inside the actual program

void main()
{
	gl_Position = u_vpm * position;
	v_position = position.xyz;
	v_texCoord = texCoord;
}