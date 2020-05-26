#include "render.h"

static UniformBuffer worldData_ubo;
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
            -0.5,  0.5, 1, 0, 0,
             0.5,  0.5, 1, 1, 0,
             0.5, -0.5, 1, 1, 1,
            -0.5, -0.5, 1, 0, 1
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
        program_construct(&rectBatch.program, "modules/std_render/shader/vertex.glsl", "modules/std_render/shader/fragment.glsl");
	}

	uint32_t bindingPoint = 1;
	glUniformBlockBinding(rectBatch.program, glGetUniformBlockIndex(rectBatch.program, "WorldBlock"), bindingPoint);
	uniformBuffer_construct(&worldData_ubo, 64, GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, worldData_ubo);
}


void render_system(checs_system_parameters)
{
	checs_component_use(Renderable, r);
	checs_component_mut_use(Camera, c);
	checs_component_use(Transform, t);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	checs_component_get(Camera, c, checs_entity_get_by_tag(CameraTag));
	camera_default_vp_recalculate(c);

	uniformBuffer_bind(worldData_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, c->vp);
	
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

	    	glm_translate(transforms[i], (vec3){r->offset[0], r->offset[1], 0});
	    	glm_rotate_z(transforms[i], r->rot, transforms[i]);
	    	glm_translate(transforms[i], (vec3){t->pos[0], t->pos[1], 0});
	    	glm_scale(transforms[i], (vec3){r->scale[0], r->scale[1], 1});

	    	textureLayers[i] = r->texture->layer;
			glm_vec2_copy(r->texture->offset, textureOffsets[i]);
			glm_vec2_copy(r->texture->size, textureSizes[i]);
		}
		vertexArray_bind(&rectBatch.vao);
		program_bind(rectBatch.program);

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
	uniformBuffer_destruct(&worldData_ubo);
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