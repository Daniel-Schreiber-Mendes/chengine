#include "movement.h"


void movement_task(void)
{
	checs_component_mut_use(Transform, t);
	checs_component_mut_use(Velocity, v);
	checs_components_foreach(Velocity, v, entity)
	{
		checs_component_get(Transform, t, entity);
		glm_vec2_scale(v->vel, 0.96, v->vel);
		glm_vec2_add(v->vel, t->pos, t->pos); //because z position is static use vec2 which only has a x and z component
	}
}


float transform_distance_get(Transform const *restrict t, Transform const *restrict t2)
{
    return sqrtf(pow(t->pos[0] - t2->pos[0], 2) + pow(t->pos[1] - t2->pos[1], 2));
}