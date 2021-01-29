#version 430

precision lowp float;
precision lowp sampler2DArray;

out vec4 color;
in vec2 v_texCoord;
flat in float layer;
flat in vec4 f_color;

uniform sampler2DArray u_texture;


void main()
{
	color = vec4(1, 1, 1, 1);//texture(u_texture, vec3(v_texCoord, layer)) * f_color;
}