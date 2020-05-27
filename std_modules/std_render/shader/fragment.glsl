#version 430

precision lowp float;
precision lowp sampler2DArray;

out vec4 color;
in vec3 v_texCoord;

uniform sampler2DArray u_textures;


void main()
{
	color = texture(u_textures, v_texCoord);
}