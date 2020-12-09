#include "render.h"

static Ubo camera_ubo;
static RectBatch rectBatch;
static EntityId camera; 


void render_task_init(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST); //is needed so a pixel with its z coordinate at -1 appers behind one with 0

	render_task_imm_init();

	{
		vao_construct(&rectBatch.vao);
		vao_bind(&rectBatch.vao);

		//rectbatch
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

	        Ebo ebo;

	        ebo_construct(&ebo, elements, sizeof(elements), GL_STATIC_DRAW, 6);
	        vbo_construct(&rectBatch.pv_vbo, positions, sizeof(positions), GL_STATIC_DRAW);
	        
	        Vbl vbl;
	    	vbl_construct(&vbl, 2);
	        vbl_element_add(&vbl, ((Vble){3, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 0, rectBatch.pv_vbo})); //pos coords
	        vbl_element_add(&vbl, ((Vble){2, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 0, rectBatch.pv_vbo})); //tex coords
	        vao_vbl_add(&rectBatch.vao, vbl);
        	vbl_destruct(&vbl);
    	}

    	{
    		//per instance
    		vbo_construct(&rectBatch.pi_vbo, NULL, 80 * 16, GL_STREAM_DRAW);

	       	Vbl vbl;
	    	vbl_construct(&vbl, 6);
	        vbl_element_add(&vbl, ((Vble){2, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, rectBatch.pi_vbo})); //texture offset
	        vbl_element_add(&vbl, ((Vble){2, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, rectBatch.pi_vbo})); //texture size

	    	vbl_element_add(&vbl, ((Vble){4, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, rectBatch.pi_vbo})); //transform row 1
	    	vbl_element_add(&vbl, ((Vble){4, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, rectBatch.pi_vbo})); //transform row 2
	    	vbl_element_add(&vbl, ((Vble){4, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, rectBatch.pi_vbo})); //transform row 3
	    	vbl_element_add(&vbl, ((Vble){4, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 1, rectBatch.pi_vbo})); //transform row 4
	        vao_vbl_add(&rectBatch.vao, vbl);
        	vbl_destruct(&vbl);
    	}

        rectBatch.mode = GL_TRIANGLES;
        program_construct(&rectBatch.program, "modules/std_render/shader/vertex.glsl", "modules/std_render/shader/fragment.glsl");
	}

	ubo_construct(&camera_ubo, 64, GL_DYNAMIC_DRAW, rectBatch.program, "Camera", 0);
}


void render_task(void)
{
	checs_component_use(Renderable, r);
	checs_component_use(Transform, t);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	bo_update(Ubo, camera_ubo, float, camera_ubo_p, ({
		camera_vp_recalculate(camera, camera_ubo_p);
	}));

	
	rectBatch.elementCount = 0;
		uint32_t textureLayers[checs_component_count(0)];
		vao_bind(&rectBatch.vao);

		bo_update(Vbo, rectBatch.pi_vbo, float, p, ({
			checs_components_foreach(0, Renderable, r, entity, ({
				checs_component_get(1, Transform, t, entity);

				float *const textureOffset = p + entityi * 20;
				float *const textureSize = p + entityi * 20 + 2;
				float *const transform = p + entityi * 20 + 4;

				glm_mat4_identity(transform);
		    	glm_translate(transform, (vec3){t->pos[0], t->pos[1], 0});
		    	glm_rotate_z(transform, r->rot, transform);
		    	glm_translate(transform, (vec3){r->offset[0], r->offset[1], 0});
		    	glm_scale(transform, (vec3){r->scale[0], r->scale[1], 1});

		    	glm_vec2_copy(&r->texture->offset, textureOffset);
		    	glm_vec2_copy(&r->texture->size, textureSize);
		    	textureLayers[entityi] = r->texture->layer;
		    	rectBatch.elementCount += 6;
		    	checs_assert(r->texture);
			}));
		}))

		program_bind(rectBatch.program);
		program_uniform1uv_set(rectBatch.program, "u_texture_layers", checs_component_count(0), textureLayers);

		textureManager_texture_bind();
		glDrawElementsInstanced(rectBatch.mode, rectBatch.elementCount, GL_UNSIGNED_INT, NULL, checs_component_count(0));

	
	render_task_on_imm_draw();

	glfwSwapBuffers(window);
}


void render_task_terminate(void)
{
	render_task_imm_terminate();
	program_destruct(rectBatch.program);
	ubo_destruct(&camera_ubo);
	vao_destruct(&rectBatch.vao);
	vbo_destruct(&rectBatch.pi_vbo);
	vbo_destruct(&rectBatch.pv_vbo);
}


void render_task_camera_set(EntityId const c)
{
	camera = c;
}