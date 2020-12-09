#version 430

in mediump vec4 v_color;
out mediump vec4 color;


void main()
{
	color = v_color;
	//color = vec4(1, 1, 1, 1);
}