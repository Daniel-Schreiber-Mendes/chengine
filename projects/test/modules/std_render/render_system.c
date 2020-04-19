#include "render.h"


static void(*custom_draw_callback)(void);

//imediate mode data
static VertexArray imm_vertexArray;
static ElementBuffer imm_elementBuffer;
static Program imm_program;


void render_system_init(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST); //is needed so a pixel with its z coordinate at -1 appers behind one with 0


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
	program_construct(&imm_program, "./resources/shader/imm_vertex.glsl", "./resources/shader/imm_fragment.glsl");
}


void render_system(checs_system_parameters)
{
	checs_component_use(Renderable, r);
	checs_component_use(Camera, c);
	checs_component_use(Transform, t);

	checs_component_get(Camera, c, checs_entity_get_by_tag(CameraTag));
	camera_default_vp_recalculate(c);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	checs_entity_foreach(entity)
	{
		checs_component_get(Renderable, r, entity);
		checs_component_get(Transform, t, entity);


		//apply transforms of the entity and upload them to the shader uniforms
		mat4 transform;
		transform_transform_calculate(t, transform);


		vertexArray_bind(&r->vao);
		program_bind(r->program);
		if (r->texture)
		{
			texture_bind(r->texture);
		}
		program_uniformMat4_set(r->program, "u_transform", transform);
		program_uniformMat4_set(r->program, "u_camera_vp", c->vp);

		switch (r->renderableType)
		{
			case ARRAYS:
				glDrawArrays(r->mode, 0, r->vertexCount);
				break;

			case ARRAYS_INSTANCED:
				glDrawArraysInstanced(r->mode, 0, r->vertexCount, r->primitiveCount);
				break;

			case ELEMENTS:
				glDrawElements(r->mode, r->elementCount, GL_UNSIGNED_INT, NULL);
				break;

			case ELEMENTS_INSTANCED:
				glDrawElementsInstanced(r->mode, r->elementCount, GL_UNSIGNED_INT, NULL, r->primitiveCount);
				break;
		}
	}
	
	if (custom_draw_callback) //used for imediate mode and debugging purposes
	{
		custom_draw_callback();
	}

	glfwSwapBuffers(window);
}


void render_system_terminate(void)
{
	vertexBuffer_destruct(&imm_vertexArray);
	elementBuffer_destruct(&imm_elementBuffer);
	program_destruct(imm_program);
}


void render_system_custom_draw_callback_set(void(*_custom_draw_callback)(void))
{
	custom_draw_callback = _custom_draw_callback;
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


void renderable_construct(Renderable *const r)
{
    vertexArray_construct(&r->vao);
}


void renderable_destruct(Renderable *const r)
{
    vertexArray_destruct(&r->vao);
}