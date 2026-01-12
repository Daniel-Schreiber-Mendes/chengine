#version 330 core

in vec2 v_position;
out vec4 color;

void main ()
{
	color = vec4(0, 0, 0, clamp(2 *  distance(v_position.xy, vec2(0.5, 0.5)), 0.1, 0.3));
}