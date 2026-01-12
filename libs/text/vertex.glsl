#version 330 core

in vec4 vertex;
out vec2 texCoords;


void main()
{
	gl_Position = vec4(vertex.xy, 0, 1);
	texCoords = vertex.zw;
}