#version 430

//per instance
in vec2 textureSize;
in vec2 size;
in vec2 offset;

out vec3 v_texCoord;


uniform mat4 u_transform;
uniform uint u_texture_layer;


layout (std140) uniform Camera
{
					//base alignment //aligned offset
	mat4 u_camera_vp; //64              0
};


vec3 positions[4] = vec3[4]
(
	vec3(-0.5f, -0.5f, 1),
	vec3(0.5f, -0,5f, 1),
	vec3(0.5f, 0.5f, 1),
	vec3(-0.5f, 0.5f, 1)
);


vec3 texturePositions[4] = vec3[4]
(
	vec3(0, 0, 1),
	vec3(1, 0, 1),
	vec3(1, 1, 1),
	vec3(0, 1, 1)
);


void main()
{
	gl_Position = u_camera_vp * u_transform * (positions[gl_VertexID % 4] * vec3(size, 1) + offset);
	v_texCoord = vec3(texturePositions[gl_VertexID % 4] * textureSize, float(u_texture_layer));
}