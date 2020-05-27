#include "render.h"


static Vao vao;
static Ebo ebo;
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

	Vbl vbl;
	Vbo vbo;
	vao_construct(&vao);
	vbo_construct(&vbo, positions, sizeof(positions), GL_STATIC_DRAW);
	ebo_construct(&ebo, elements, sizeof(elements), GL_STATIC_DRAW, 6);

	vbl_construct(&vbl, 1);
	vbl_element_add(&vbl, ((Vble){2, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 0, vbo}));
	vao_vbl_add(&vao, vbl);
	vbl_destruct(&vbl);
	program_construct(&program, "modules/std_render/shader/imm_vertex.glsl", "modules/std_render/shader/imm_fragment.glsl");
}


void render_system_imm_terminate(void)
{
	vbo_destruct(&vao);
	ebo_destruct(&ebo);
	program_destruct(program);
}


void render_system_imm_rectangle_draw(vec4 const color, vec3 pos, vec2 const size)
{
	//imediate mode drawing is pretty slow compared to retained mode drawing but this is not a problem because it is only used for debugging purposes
	vao_bind(&vao);
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

	glDrawElements(GL_TRIANGLES, ebo.elementCount, GL_UNSIGNED_INT, NULL);
}