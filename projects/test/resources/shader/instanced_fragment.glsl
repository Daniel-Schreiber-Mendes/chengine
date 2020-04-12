#version 300 es

out mediump vec4 color;
in mediump vec2 texture_pos;

uniform sampler2D u_texture;

void main()
{
	color = texture(u_texture, texture_pos);
}