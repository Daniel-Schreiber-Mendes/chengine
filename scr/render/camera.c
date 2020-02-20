#include "render.h"

void camera_construct(Camera *const c, float const left, float const right, float const bottom, float const top)
{
	c->rotation = 0;
	glm_mat4_identity(c->viewm);
	glm_mat4_identity(c->vpm);
	glm_ortho(left, right, bottom, top, -1.f, 1.f, c->projm);
	camera_vpm_recalculate(c);
}


void camera_vpm_recalculate(Camera *const c)
{
	glm_mat4_identity(c->viewm); //reset the view matrix
	glm_rotate(c->viewm, c->rotation, (vec3){0, 1, 0});  //rotate it

	glm_translate(c->viewm, c->position); //moving the camera
	glm_mat4_inv_fast(c->viewm, c->viewm); //inverting the coords cause if the camera should move to the right everything moves left
	glm_mat4_mul(c->projm, c->viewm, c->vpm);
	/*inverting the matrix because if the camera is moving to the right, everything should move to the left.
	this means every transformation should at the end be inversed*/
}