#include "movement.h"


void movement_task(void)
{
	checs_component_mut_use(Transform, t);
	checs_component_mut_use(Velocity, v);

	checs_components_foreach(5, Velocity, v, entity, ({
		checs_component_get(1, Transform, t, entity);
		glm_vec2_scale(v->vel, 0.96, v->vel);
		glm_vec2_add(v->vel, t->pos, t->pos);
	}));
}


float transform_distance_get(Transform const *restrict t, Transform const *restrict t2)
{
    return sqrtf(pow(t->pos[0] - t2->pos[0], 2) + pow(t->pos[1] - t2->pos[1], 2));
}