#version 300 es


uniform mat4 u_transform;
uniform mat4 u_camera_vp; 
uniform float u_tile_size; 
/*width and height of tile relative to the whole size of the image. 
if the tilemap contains for example 10 tiles and each tile has a size 32 pixels, the tile size is 32 / 320 */
uniform uint u_texture_index;
uniform uint u_chunk_tile_count; //number of tiles per row per chunk

out vec2 texture_pos;
 
const vec2 positions[6] = vec2[6]
(
	vec2(0.0, 0.0),
	vec2(1.0, 0.0),
	vec2(1.0, 1.0),
	vec2(1.0, 1.0),
	vec2(0.0, 1.0),
	vec2(0.0, 0.0)
);


void main()
{
	float column = mod(float(gl_VertexID / 6), float(u_chunk_tile_count));
	float row = float((gl_VertexID / 6) / int(u_chunk_tile_count));
    gl_Position = u_camera_vp * u_transform * vec4(positions[gl_VertexID % 6].x + column, -(positions[gl_VertexID % 6].y + row), 0, 1);
    texture_pos = vec2((u_tile_size) * (positions[gl_VertexID % 6].x + float(u_texture_index)), (u_tile_size) * positions[gl_VertexID % 6].y);
}