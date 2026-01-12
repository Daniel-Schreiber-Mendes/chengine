#version 430

precision lowp float;
precision lowp sampler2DArray;

out vec4 color;

in vec2 v_texCoord;
in vec2 v_position;

uniform sampler2D u_textures;
uniform Camera { mat4 u_camera_vp; };


void main()
{

	vec4 texColor = texture(u_textures, v_texCoord);

	//if there are any remains that are not totally opaque, discard the fragementu
	if (texColor.a < 0.05)
		discard;

	color = texColor;
}