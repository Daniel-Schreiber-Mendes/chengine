#version 300 es

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord; //v_ prefix stands for variing. these variables are passed between shaders

uniform mat4 u_transform;
uniform mat4 u_camera_vp; //u_ prefix stands for uniform. these variables are set inside the actual program

void main()
{
	gl_Position = u_camera_vp * u_transform * vec4(position.x, position.y, position.z, 1);
	v_texCoord = texCoord;
}