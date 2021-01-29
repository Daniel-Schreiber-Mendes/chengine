#version 300 es

precision lowp float;

out vec4 color;
uniform vec4 u_color;


void main()
{
	color = u_color;
}
