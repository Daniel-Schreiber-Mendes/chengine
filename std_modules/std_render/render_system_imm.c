#include "render.h"


static VertexArray vertexArray;
static ElementBuffer elementBuffer;
static Program program;


void render_system_imm_init(void)
{
	GLfloat positions[8] =
	{
		-1, -1,
		 1, -1,
		 1,  1,
		-1,  1, 
	};

	GLuint elements[6] = 
	{
		0, 1, 2,
		2, 3, 0
	};

	VertexBufferLayout vbl;
	VertexBuffer vbo;
	vertexArray_construct(&vertexArray);
	vertexBuffer_construct(&vbo, positions, sizeof(positions), GL_STATIC_DRAW);
	elementBuffer_construct(&elementBuffer, elements);

	vertexBufferLayout_construct(&vbl, 1);
	vertexBufferLayout_element_add(&vbl, ((VertexBufferLayoutElement){2, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 0, vbo}));
	vertexArray_vbl_add(&vertexArray, vbl);
	vertexBufferLayout_destruct(&vbl);
	program_construct(&program, "modules/std_render/shader/imm_vertex.glsl", "modules/std_render/shader/imm_fragment.glsl");
}


void render_system_imm_terminate(void)
{
	vertexBuffer_destruct(&vertexArray);
	elementBuffer_destruct(&elementBuffer);
	program_destruct(program);
}


void render_system_imm_rectangle_draw(vec4 const color, vec3 pos, vec2 const size)
{
	//imediate mode drawing is pretty slow compared to retained mode drawing but this is not a problem because it is only used for debugging purposes
	vertexArray_bind(&vertexArray);
	program_bind(program);
	checs_component_get_once(Camera, c, checs_entity_get_by_tag(CameraTag));

	mat4 transform, proj;
	glm_mat4_identity(transform);
	glm_scale(transform, (vec3){size[0], size[1], 1});
	glm_translate(transform, pos);
	glm_ortho(-c->aspectRatio, c->aspectRatio, -1, 1, -1.f, 1.f, proj); 
	//only the aspect ratio is important for the projection because this drawable should not be manipulated by zoom or rotation or translation

	program_uniformMat4_set(program, "u_proj", proj);
	program_uniform4fv_set(program, "u_color", color);
	program_uniformMat4_set(program, "u_transform", transform);

	glDrawElements(GL_TRIANGLES, elementBuffer.elementCount, GL_UNSIGNED_INT, NULL);
}