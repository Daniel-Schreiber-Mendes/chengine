#version 300 es

layout(location = 0) in vec2 position;

out vec4 v_color;
uniform mat4 u_proj; 
uniform mat4 u_transform;
uniform vec4 u_color;

void main()
{
	gl_Position = u_proj * u_transform * vec4(position.x, position.y, 0, 1);
	v_color = u_color;
}