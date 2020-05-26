#version 300 es

layout(location = 0) in vec3 position;

uniform mat4 u_transform;
uniform mat4 u_camera_vp; //u_ prefix stands for uniform. these variables are set inside the actual program

void main()
{
	gl_Position = u_camera_vp * u_transform * vec4(position.x, position.y, position.z, 1);
}