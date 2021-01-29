#include "render.h"

/*
static Vao rect_vao, text_vao;
static Vbo rect_vbo, text_vbo;
static Ebo rect_ebo, text_ebo;
static Program rect_program, text_program;
static Ssbo text_ssbo;
*/

void render_task_imm_init(void)
{/*
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

	{
		Vbl vbl;
		vao_construct(&rect_vao);
		vbo_construct(&text_vbo, positions, sizeof(positions), GL_STATIC_DRAW);
		ebo_construct(&rect_ebo, elements, sizeof(elements), GL_STATIC_DRAW, 6);

		vbl_construct(&vbl, 1);
		vbl_element_add(&vbl, ((Vble){2, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 0, text_vbo}));
		vao_vbl_add(&rect_vao, vbl);
		vbl_destruct(&vbl);
		program_construct(&rect_program, "modules/std_render/shader/imm_rect.vs", "modules/std_render/shader/imm_rect.fs");
	}
	
	{
		Vbl vbl;
		vao_construct(&text_vao);
		vbo_construct(&text_vbo, NULL, 44 * 64, GL_DYNAMIC_DRAW);
		ebo_construct(&text_ebo, elements, sizeof(elements), GL_STATIC_DRAW, 6);

		vbl_construct(&vbl, 6);
		vbl_element_add(&vbl, ((Vble){2, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, text_vbo})); //scale
		vbl_element_add(&vbl, ((Vble){2, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, text_vbo})); //pos
		vbl_element_add(&vbl, ((Vble){4, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, text_vbo})); //color
		vbl_element_add(&vbl, ((Vble){1, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, text_vbo})); //currentfont
		vbl_element_add(&vbl, ((Vble){1, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, text_vbo})); //stringsize
		vbl_element_add(&vbl, ((Vble){64, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, text_vbo})); //string
		vao_vbl_add(&text_vao, vbl);
		vbl_destruct(&vbl);
		program_construct(&text_program, "modules/std_render/shader/imm_text.vs", "modules/std_render/shader/imm_text.fs");
	}

	
	ssbo_construct(&text_ssbo, 5144, 0, GL_DYNAMIC_DRAW);*/
}


void render_task_imm_terminate(void)
{/*
	vao_destruct(&rect_vao);
	ebo_destruct(&rect_ebo);
	program_destruct(rect_program);

	vao_destruct(&text_vao);
	ebo_destruct(&text_ebo);
	program_destruct(text_program);
	ssbo_destruct(&text_ssbo);*/
}


void render_task_imm_rectangle_draw(vec4 const color, vec3 const pos, vec2 const size)
{/*
	//imediate mode drawing is pretty slow compared to retained mode drawing but this is not a problem because it is only used for debugging purposes
	vao_bind(&rect_vao);
	program_bind(rect_program);
	checs_component_get_once(<Camera>, Camera, c, checs_entity_get_by_tag(CameraTag));

	mat4 transform, proj;
	glm_mat4_identity(transform);
	glm_scale(transform, (vec3){size[0], size[1], 1});
	glm_translate(transform, pos);
	glm_ortho(-c->aspectRatio, c->aspectRatio, -1, 1, -1.f, 1.f, proj); 
	//only the aspect ratio is important for the projection because this drawable should not be manipulated by zoom or rotation

	program_uniformMat4_set(rect_program, "u_proj", proj);
	program_uniform4fv_set(rect_program, "u_color", color);
	program_uniformMat4_set(rect_program, "u_transform", transform);

	glDrawElements(GL_TRIANGLES, rect_ebo.elementCount, GL_UNSIGNED_INT, NULL);*/
}


void render_task_imm_text_draw(vec4 const color, vec2 const pos, Text const *const t)
{/*
	vao_bind(&text_vao);
	program_bind(text_program);


	bo_update(Vbo, text_vbo, float, p, ({
		p[0] = t->scale;
		glm_vec2_copy(pos, &p[1]);
		glm_vec4_ucopy(color, &p[3]);
		p[7] = t->font->index;

		for (uint8_t i=0; t->string[i] != '\0'; ++i)
			p[8 + i] = t->string[i];
	}))
	
	glDrawElements(GL_TRIANGLES, text_ebo.elementCount, GL_UNSIGNED_INT, NULL);*/
}


void render_task_imm_font_load(Font *const font)
{/*
	bo_update(Ssbo, text_ssbo, float, fonts, ({

		float *valids = fonts + 5136;

		for (uint8_t i=0; i < 2; ++i)
		{
			if (!(bool)valids[i])
			{
				font->index = i;

				float *textureSize = fonts + 2568 * i;
				textureSize[0] = font->textureSize[0];
				textureSize[1] = font->textureSize[1];

				float **characters = textureSize + 8;
				for (uint8_t j=0; j < 128; ++i)
				{
					characters[j * 5][0] = font->characters[i].size[0];
					characters[j * 5][1] = font->characters[i].size[1];
					characters[j * 5][2] = font->characters[i].bearing[0];
					characters[j * 5][3] = font->characters[i].bearing[1];
					characters[j * 5][4] = font->characters[i].advance;
				}
			}
			break;
		}
	}))*/
}