#include "render.h"


static RectBatch rectBatch;
/*
static void *batches[1] =
{
	&rectBatch
}; //array of pointers to different batch types
*/

void render_system_init(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST); //is needed so a pixel with its z coordinate at -1 appers behind one with 0

	render_system_imm_init();
	
	{
		vector_construct(&rectBatch.entitys, sizeof(EntityId));
		vertexArray_construct(&rectBatch.vao);
		vertexArray_bind(&rectBatch.vao);

		GLfloat positions[5 * 4] = 
        {//  3 x position  2 x tex
            0,  0, 1, 0, 0,
            1,  0, 1, 1, 0,
            1, -1, 1, 1, 1,
            0, -1, 1, 0, 1
        };

        VertexBuffer vbo;
        vertexBuffer_construct(&vbo, positions);


        GLuint elements[6] = 
        {
            0, 1, 2,
            2, 3, 0
        };

        ElementBuffer ebo;
        elementBuffer_construct(&ebo, elements);

        {
        	VertexBufferLayout vbl;
        	vertexBufferLayout_construct(&vbl, 2);

	        vertexBufferLayout_element_add(&vbl, ((VertexBufferLayoutElement){3, GL_FLOAT, GL_FALSE})); //pos coords
	        vertexBufferLayout_element_add(&vbl, ((VertexBufferLayoutElement){2, GL_FLOAT, GL_FALSE})); //tex coords
	        vertexArray_buffer_add(&rectBatch.vao, vbo, vbl);

	        vertexBufferLayout_destruct(&vbl);
        }

        rectBatch.mode = GL_TRIANGLES;
        program_construct(&rectBatch.program, "./resources/shader/vertex.glsl", "./resources/shader/fragment.glsl");
	}
}


void render_system(checs_system_parameters)
{
	checs_component_use(Renderable, r);
	checs_component_mut_use(Camera, c);
	checs_component_use(Transform, t);

	checs_component_get(Camera, c, checs_entity_get_by_tag(CameraTag));
	camera_default_vp_recalculate(c);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	checs_entity_foreach(entity)
	{
		checs_component_get(Renderable, r, entity);
		checs_component_get(Transform, t, entity);

		//apply transforms of the entity and upload them to the shader uniforms
		mat4 transform;
		glm_mat4_identity(transform);
    	glm_scale_uni(transform, r->scale);
    	glm_rotate_z(transform, r->rot, transform);
    	glm_translate(transform, t->pos);

		vertexArray_bind(&r->vao);
		program_bind(r->program);

		program_uniformMat4_set(r->program, "u_transform", transform);
		program_uniformMat4_set(r->program, "u_camera_vp", c->vp);
		program_uniform1u_set(r->program, "u_texture_layer", r->texture->layer);

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
	}*/
	
	if (rectBatch.entitys.size > 0)
	{
		uint32_t textureLayers[rectBatch.entitys.size];
		vec2 textureSizes[rectBatch.entitys.size];
		vec2 textureOffsets[rectBatch.entitys.size];
		mat4 transforms[rectBatch.entitys.size];

		glm_mat4_identity_array(transforms, rectBatch.entitys.size);

		vector_foreach(&rectBatch.entitys, EntityId, entity)
		{
			checs_component_get(Renderable, r, entity);
			checs_component_get(Transform, t, entity);

	    	glm_scale_uni(transforms[i], r->scale);
	    	glm_rotate_z(transforms[i], r->rot, transforms[i]);
	    	glm_translate(transforms[i], t->pos);

	    	textureLayers[i] = r->texture->layer;
			glm_vec2_copy(r->textureOffset, textureOffsets[i]);
			glm_vec2_copy((vec2){r->texture->width / (float)LAYER_WIDTH, r->texture->height / (float)LAYER_HEIGHT}, textureSizes[i]);
		}
		vertexArray_bind(&rectBatch.vao);
		program_bind(rectBatch.program);

		checs_component_get(Camera, c, checs_entity_get_by_tag(CameraTag));
		camera_default_vp_recalculate(c);

		program_uniformMat4_set(rectBatch.program, "u_camera_vp", c->vp);
		program_uniformMat4v_set(rectBatch.program, "u_transforms", transforms, rectBatch.entitys.size);
		program_uniform2fv_set(rectBatch.program, "u_texture_offsets", rectBatch.entitys.size, textureOffsets);
		program_uniform1uv_set(rectBatch.program, "u_texture_layers", rectBatch.entitys.size, textureLayers);
		program_uniform2fv_set(rectBatch.program, "u_texture_sizes", rectBatch.entitys.size, textureSizes);
		glDrawElementsInstanced(rectBatch.mode, rectBatch.elementCount, GL_UNSIGNED_INT, NULL, rectBatch.entitys.size);
	}

	
	render_system_imm_render();

	glfwSwapBuffers(window);
}


void render_system_terminate(void)
{
	render_system_imm_terminate();
	vector_destruct(&rectBatch.entitys);
}


void render_system_on_entity_added(EntityId const e)
{
	checs_component_get_once(Renderable, r, e);
	if (r->type == RT_RECT)
	{
		vector_push_back(&rectBatch.entitys, EntityId, e);
		rectBatch.elementCount += 6;
	}
}