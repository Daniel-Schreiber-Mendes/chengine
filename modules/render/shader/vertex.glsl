#version 430

//per vertex
in vec3 position;
in vec2 texCoord;
//per instance
in vec4 r1, r2, r3, r4;
in vec2 textureSize;
in vec2 texture_pos;

out vec2 v_texCoord;
out vec2 v_position;

uniform Camera { mat4 u_camera_vp; };



void main()
{
	gl_Position = u_camera_vp * mat4(r1, r2, r3, r4) * vec4(position, 1); 
	v_texCoord = texCoord * textureSize + texture_pos; 
	v_position = gl_Position.xy;
}