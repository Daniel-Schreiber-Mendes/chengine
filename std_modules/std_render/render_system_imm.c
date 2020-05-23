#include "render.h"


static void(*on_imm_draw)(void);
static VertexArray imm_vertexArray;
static ElementBuffer imm_elementBuffer;
static Program imm_program;


void render_system_imm_init(void)
{
	GLfloat imm_positions[8] =
	{
		-1, -1,
		 1, -1,
		 1,  1,
		-1,  1, 
	};

	GLuint imm_elements[6] = 
	{
		0, 1, 2,
		2, 3, 0
	};

	VertexBufferLayout imm_vbl;
	VertexBuffer imm_vbo;
	vertexArray_construct(&imm_vertexArray);
	vertexBuffer_construct(&imm_vbo, imm_positions);
	elementBuffer_construct(&imm_elementBuffer, imm_elements);

	vertexBufferLayout_construct(&imm_vbl, 1);
	vertexBufferLayout_element_add(&imm_vbl, ((VertexBufferLayoutElement){2, GL_FLOAT, GL_FALSE}));
	vertexArray_buffer_add(&imm_vertexArray, imm_vbo, imm_vbl);
	vertexBufferLayout_destruct(&imm_vbl);
	program_construct(&imm_program, "./resources/shader/imm_vertex.glsl", "./resources/shader/imm_fragment.glsl");
}


void render_system_imm_terminate(void)
{
	vertexBuffer_destruct(&imm_vertexArray);
	elementBuffer_destruct(&imm_elementBuffer);
	program_destruct(imm_program);
}


void render_system_imm_render(void)
{
	if (on_imm_draw) //used for imediate mode and debugging purposes
	{
		on_imm_draw();
	}
}


void render_system_imm_on_imm_draw_set(void(*_on_imm_draw)(void))
{
	on_imm_draw = _on_imm_draw;
}


void render_system_imm_rectangle_draw(vec4 const color, vec3 pos, vec2 const size)
{
	//imediate mode drawing is pretty slow compared to retained mode drawing but this is not a problem because it is only used for debugging purposes
	vertexArray_bind(&imm_vertexArray);
	program_bind(imm_program);
	checs_component_get_once(Camera, c, checs_entity_get_by_tag(CameraTag));

	mat4 transform, proj;
	glm_mat4_identity(transform);
	glm_scale(transform, (vec3){size[0], size[1], 1});
	glm_translate(transform, pos);
	glm_ortho(-c->aspectRatio, c->aspectRatio, -1, 1, -1.f, 1.f, proj); 
	//only the aspect ratio is important for the projection because this drawable should not be manipulated by zoom or rotation or translation

	program_uniformMat4_set(imm_program, "u_proj", proj);
	program_uniform4fv_set(imm_program, "u_color", color);
	program_uniformMat4_set(imm_program, "u_transform", transform);

	glDrawElements(GL_TRIANGLES, imm_elementBuffer.elementCount, GL_UNSIGNED_INT, NULL);
}