#version 300 es

out mediump vec4 color;
in mediump vec2 position;


uniform sampler2D u_texture;
//uniform uint u_texture_index;
uniform mediump float u_texture_size; //width of image. width and height have to be the same
uniform mediump float u_tile_size;


void main()
{
	//vec2(u_tile_size / u_texture_size, u_tile_size / u_texture_size) * position
	color = texture(u_texture, vec2(u_tile_size / u_texture_size, u_tile_size / u_texture_size) * position);
}