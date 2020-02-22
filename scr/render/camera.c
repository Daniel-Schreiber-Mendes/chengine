#include "render.h"

void camera_construct(Transform *const c, float const left, float const right, float const bottom, float const top)
{/*
	c->rotation = 0;
	mat4 view, proj, vp; 
	glm_mat4_identity(view);
	glm_mat4_identity(vpm);
	glm_ortho(left, right, bottom, top, -1.f, 1.f, projm);
	camera_vpm_recalculate(c);*/
}


void camera_vpm_recalculate(Transform *const c)
{
	mat4 view, proj, vp; 
	glm_mat4_identity(view); //reset the view matrix
	glm_mat4_identity(proj);
	glm_mat4_identity(vp);

	glm_translate(view, c->position); //moving the camera
	glm_rotate(view, c->rotation, (vec3){0, 0, 1});  //rotate it
	glm_mat4_inv_fast(view, view); //inverting the coords cause if the camera should move to the right everything moves left
	glm_mat4_mul(proj, view, vp);
	/*inverting the matrix because if the camera is moving to the right, everything should move to the left.
	this means every transformation should at the end be inversed*/
}