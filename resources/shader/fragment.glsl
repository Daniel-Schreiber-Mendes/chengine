#version 300 es

out mediump vec4 color;
in mediump vec2 v_texCoord;
in mediump vec3 v_position;

uniform sampler2D u_texture;


void main()
{
	color = texture(u_texture, v_texCoord);
	//color = vec4(v_position + vec3(0.2, 0.2, 0.2), 0);
}