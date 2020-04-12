#version 300 es

out mediump vec4 color;
in mediump vec2 v_texCoord;

uniform sampler2D u_texture;


void main()
{
	color = texture(u_texture, v_texCoord);
}