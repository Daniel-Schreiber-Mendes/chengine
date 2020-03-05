#version 300 es

layout(location = 0) in float unused_attributes;

uniform mat4 u_transform;
uniform mat4 u_camera_vp; 
uniform float u_tile_size; 
/*width and height of tile relative to the whole size of the image. 
if the tilemap contains for example 10 tiles and each tile has a size 32 pixels, the tile size is 32 / (320)*/
uniform uint u_texture_index;
uniform uint u_chunk_size;

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
	float column = mod(float(gl_InstanceID), float(u_chunk_size));
	float row = float(gl_InstanceID / int(u_chunk_size));
    gl_Position = u_camera_vp * u_transform * vec4(positions[gl_VertexID].x + column, positions[gl_VertexID].y - row, 0, 1);
    texture_pos = vec2((u_tile_size) * (positions[gl_VertexID].x + float(u_texture_index)), (u_tile_size) * positions[gl_VertexID].y);
}