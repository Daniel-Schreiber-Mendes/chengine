#version 300 es

layout(location = 0) in vec3 unused;

uniform mat4 u_transform;
uniform mat4 u_camera_vp; //u_ prefix stands for uniform. these variables are set inside the actual program

out vec2 position;
 
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
    position = positions[gl_VertexID];
    gl_Position = u_camera_vp * u_transform * vec4(position.x, position.y, 0, 1);
}