#include "render.h"

static UniformBuffer cameraData_ubo;
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

        {
        	//per vertex
			GLfloat positions[5 * 4] = 
	        {//  3 x position  2 x tex
	            -0.5,  0.5, 1, 0, 0,
	             0.5,  0.5, 1, 1, 0,
	             0.5, -0.5, 1, 1, 1,
	            -0.5, -0.5, 1, 0, 1
	        };

	        GLuint elements[6] = 
	        {
	            0, 1, 2,
	            2, 3, 0
	        };

	        ElementBuffer ebo;

	        elementBuffer_construct(&ebo, elements);
	        vertexBuffer_construct(&rectBatch.pv_vbo, positions, sizeof(positions), GL_STATIC_DRAW);
	        
	        VertexBufferLayout vbl;
	    	vertexBufferLayout_construct(&vbl, 2);
	        vertexBufferLayout_element_add(&vbl, ((VertexBufferLayoutElement){3, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 0, rectBatch.pv_vbo})); //pos coords
	        vertexBufferLayout_element_add(&vbl, ((VertexBufferLayoutElement){2, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 0, rectBatch.pv_vbo})); //tex coords
	        vertexArray_vbl_add(&rectBatch.vao, vbl);
        	vertexBufferLayout_destruct(&vbl);
    	}

    	{
    		//per instance
    		vertexBuffer_construct(&rectBatch.pi_vbo, NULL, 80 * 16, GL_STREAM_DRAW);

	       	VertexBufferLayout vbl;
	    	vertexBufferLayout_construct(&vbl, 6);
	        vertexBufferLayout_element_add(&vbl, ((VertexBufferLayoutElement){2, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, rectBatch.pi_vbo})); //texture offset
	        vertexBufferLayout_element_add(&vbl, ((VertexBufferLayoutElement){2, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, rectBatch.pi_vbo})); //texture size

	    	vertexBufferLayout_element_add(&vbl, ((VertexBufferLayoutElement){4, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, rectBatch.pi_vbo})); //transform row 1
	    	vertexBufferLayout_element_add(&vbl, ((VertexBufferLayoutElement){4, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, rectBatch.pi_vbo})); //transform row 2
	    	vertexBufferLayout_element_add(&vbl, ((VertexBufferLayoutElement){4, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, rectBatch.pi_vbo})); //transform row 3
	    	vertexBufferLayout_element_add(&vbl, ((VertexBufferLayoutElement){4, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, rectBatch.pi_vbo})); //transform row 4
	        //vertexBufferLayout_element_add(&vbl, ((VertexBufferLayoutElement){1, sizeof(GLuint), GL_UNSIGNED_INT, GL_FALSE, 1, rectBatch.pi_vbo})); //texture layer*/
	        vertexArray_vbl_add(&rectBatch.vao, vbl);
        	vertexBufferLayout_destruct(&vbl);
    	}

        rectBatch.mode = GL_TRIANGLES;
        program_construct(&rectBatch.program, "modules/std_render/shader/vertex.glsl", "modules/std_render/shader/fragment.glsl");
	}


	uniformBuffer_construct(&cameraData_ubo, 64, GL_STREAM_DRAW, rectBatch.program, "CameraData", 0);
}


void render_system(checs_system_parameters)
{
	checs_component_use(Renderable, r);
	checs_component_use(Transform, t);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	checs_component_get_once(Camera, c, checs_entity_get_by_tag(CameraTag));

	uniformBuffer_update_begin(cameraData_ubo, cameraData);
		camera_vp_recalculate(c, cameraData);
	uniformBuffer_update_end();

	
	if (rectBatch.entitys.size > 0)
	{
		uint32_t textureLayers[rectBatch.entitys.size];

		vertexArray_bind(&rectBatch.vao);
		vector_foreach(&rectBatch.entitys, EntityId, entity)
		{
			checs_component_get(Renderable, r, entity);
			checs_component_get(Transform, t, entity);

			vertexBuffer_update_begin(rectBatch.pi_vbo, i * 80 + 16, transform);
			glm_mat4_identity(transform);
		    	glm_translate(transform, (vec3){t->pos[0], t->pos[1], 0});
		    	glm_rotate_z(transform, r->rot, transform);
		    	glm_translate(transform, (vec3){r->offset[0], r->offset[1], 0});
		    	glm_scale(transform, (vec3){r->scale[0], r->scale[1], 1});
			vertexBuffer_update_end();

	    	textureLayers[i] = r->texture->layer;
			//glNamedBufferSubData(rectBatch.pi_vbo, i * 80 + 16, 64, &transforms[i]);
			glNamedBufferSubData(rectBatch.pi_vbo, i * 80, 8, &r->texture->offset);
			glNamedBufferSubData(rectBatch.pi_vbo, i * 80 + 8, 8, &r->texture->size);

		}

		vertexArray_bind(&rectBatch.vao);
		program_bind(rectBatch.program);

		program_uniform1uv_set(rectBatch.program, "u_texture_layers", rectBatch.entitys.size, textureLayers);
		glDrawElementsInstanced(rectBatch.mode, rectBatch.elementCount, GL_UNSIGNED_INT, NULL, rectBatch.entitys.size);
	}

	
	render_system_on_imm_draw();

	glfwSwapBuffers(window);
}


void render_system_terminate(void)
{
	render_system_imm_terminate();
	vector_destruct(&rectBatch.entitys);
	uniformBuffer_destruct(&cameraData_ubo);
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