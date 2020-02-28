#include "../../engine.h"


void render_init(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void render_system(checs_system_parameters)
{
	checs_component_use(Renderable, r);
	checs_component_use(Camera, c);
	checs_component_use(Transform, t);
	mat4 view, proj, vp; 
	glClear(GL_COLOR_BUFFER_BIT);


	glm_mat4_identity(view); //reset the view matrix
	glm_mat4_identity(vp);
	
	checs_components_foreach(Camera, c, camera)
	{
		checs_component_get(Transform, t, camera);
		checs_component_get(Camera, c, camera);
		glm_ortho(-c->zoom * c->aspectRatio, c->zoom * c->aspectRatio, -c->zoom, c->zoom, -1.f, 1.f, proj); 
		/* by setting the projection always according to the aspect ratio if the window gets resized everything still gets rendered correctly*/
		glm_translate(view, t->position); //moving the camera
		glm_scale_uni(view, c->zoom);
		glm_rotate_z(view, t->rotation, view);  //rotate it
		glm_mat4_inv(view, view);
		glm_mat4_mul(proj, view, vp);

		checs_component_get(Renderable, r, 0);
		program_uniformMat4f_set(r->program, "u_vp", vp);

		checs_entity_foreach(renderable)
		{
			checs_component_get(Renderable, r, renderable);

			vertexArray_bind(r->vao);
			program_bind(r->program);

			glDrawElements(GL_TRIANGLES, r->ebo.elementCount, GL_UNSIGNED_INT, NULL);
		}
	}

	glfwSwapBuffers(window);
}