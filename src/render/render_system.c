#include "../../engine.h"


void render_system_init(void)
{
	glClearColor(0.3, 0.4, 0.5, 1);
}


void render_system(checs_system_parameters)
{
	checs_component_use(Renderable, r);
	glClear(GL_COLOR_BUFFER_BIT);

	EntityId e = 0;
	checs_component_get(Renderable, r, e);

	mat4 view, proj, vp; 
	glm_mat4_identity(view); //reset the view matrix
	glm_mat4_identity(vp);
	glm_ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.f, 1.f, proj);
	
	checs_component_get_once(Transform, t, checs_entity_get_by_tag(CameraTag));
	glm_translate(view, t->position); //moving the camera
	glm_rotate(view, t->rotation, (vec3){0, 0, 1});  //rotate it
	glm_mat4_inv(view, view);
	glm_mat4_mul(proj, view, vp);

	program_uniformMat4f_set(r->program, "u_vp", vp);

	checs_entity_foreach(entity)
	{
		checs_component_get(Renderable, r, entity);

		vertexArray_bind(r->vao);
		program_bind(r->program);

		glDrawElements(GL_TRIANGLES, r->ebo.elementCount, GL_UNSIGNED_INT, NULL);
	}

	glfwSwapBuffers(window);
}