#include "movement.h"


void movement_task(void)
{
	checs_component_use(Transform, t); //because every entity who has a velocity component also has a transform component, we only need a task, not a system
	checs_component_use(Velocity, v);
	checs_components_foreach(Velocity, v, entity)
	{
		checs_component_get(Transform, t, entity);
		v->vel[0] = (fabs(v->vel[0]) > 0.001f) ? v->vel[0] * 0.75f : 0;
		v->vel[1] = (fabs(v->vel[1]) > 0.001f) ? v->vel[1] * 0.75f : 0;
		glm_vec3_add(v->vel, t->pos, t->pos); //because we dont want to change the z position use vec2 which only has a x and z component
	}
}


void transform_transform_calculate(Transform *const t, mat4 transform)
{
    glm_mat4_identity(transform);
    glm_rotate_z(transform, t->rot, transform);
    glm_translate(transform, t->pos);
}


float transform_distance_get(Transform const *restrict t, Transform const *restrict t2)
{
    //calculating distance with pythagoreas
    return sqrtf(pow(t->pos[0] - t2->pos[0], 2) + pow(t->pos[1] - t2->pos[1], 2));
}


bool transform_circle_circle_collision(Transform const *restrict t0, Transform const *restrict t1, float const r0, float const r1)
{
    return pow(r0 + r1, 2) > (pow(t0->pos[0] + r0 - t1->pos[0] - r1, 2) + pow(t0->pos[1] + r0 - t1->pos[1] - r1, 2));
}


bool transform_circle_point_collision(Transform const *restrict t0, Transform const *restrict t1, float const r0)
{
    return transform_distance_get(t0, t1) < r0;
}


bool transform_rect_rect_collision(Transform const *restrict t0, Transform const *restrict t1, vec2 const bb0, vec2 const bb1)
{
    return t0->pos[0] + bb0[0] > t1->pos[0] && t0->pos[0] < t1->pos[0] + bb1[0] && 
           t0->pos[1] - bb0[1] < t1->pos[1] && t0->pos[1] > t1->pos[1] - bb1[1];
}


//this pushes only the first entity out of the second
void transform_rect_rect_single_bump(Transform *restrict t0, Transform *restrict t1, vec2 const bb0, vec2 const bb1)
{
    float const right =  (t0->pos[0] + bb0[0]) - t1->pos[0];
    float const left =   (t1->pos[0] + bb1[0]) - t0->pos[0];
    float const bottom = (t0->pos[1] + bb0[1]) - t1->pos[1];
    float const top =    (t1->pos[1] + bb1[1]) - t0->pos[1];
    
    if (right < left && right < top && right < bottom)
    {
        //right collision
        t0->pos[0] = t1->pos[0] - bb0[0];
    }
    else if (left < top && left < bottom)
    {
        //left collision
        t0->pos[0] = t1->pos[0] + bb1[0];
    }
    else if (top < bottom)
    {
        //top collision
        t0->pos[1] = t1->pos[1] + bb1[1];
    }
    else
    {
        //bottom collision
        t0->pos[1] = t1->pos[1] - bb0[1];
    }
}

//this pushes both entitys by the same amount away from each other
void transform_rect_rect_equal_bump(Transform *restrict t0, Transform *restrict t1, vec2 const bb0, vec2 const bb1)
{
    //TODO: implement it :)
}


bool  transform_rect_point_collision(Transform const *restrict t0, Transform const *restrict t1, vec2 const bb0)
{
    return t1->pos[0] > t0->pos[0] && t1->pos[0] < t0->pos[0] + bb0[0] && t1->pos[1] > t0->pos[1] && t1->pos[1] < t0->pos[1] + bb0[1];
}