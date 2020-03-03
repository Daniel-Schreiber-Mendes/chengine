#version 300 es

layout(location = 0) in float unused_attributes;

uniform mat4 u_transform;
uniform mat4 u_camera_vp; //u_ prefix stands for uniform. these variables are set inside the actual program
uniform float u_texture_size; //width of image. width and height have to be the same
uniform float u_tile_size;
uniform uint u_texture_index;

out vec2 texture_pos;
 
vec2 positions[6] = vec2[6]
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
	vec2 position = positions[gl_VertexID];
    gl_Position = u_camera_vp * u_transform * vec4(position.x, position.y, 0, 1);
    texture_pos = vec2((u_tile_size / u_texture_size) * position.x * float(u_texture_index), (u_tile_size / u_texture_size) * position.y);
}