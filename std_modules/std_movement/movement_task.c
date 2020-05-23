#include "movement.h"


void movement_task(void)
{
	checs_component_use(Transform, t); //because every entity who has a velocity component also has a transform component, we only need a task, not a system
	checs_component_use(Velocity, v);
	checs_components_foreach(Velocity, v, entity)
	{
		checs_component_get(Transform, t, entity);
		glm_vec2_scale(v->vel, 0.96, v->vel);
		glm_vec3_add(v->vel, t->pos, t->pos); //because we dont want to change the z position use vec2 which only has a x and z component
	}
}


float transform_distance_get(Transform const *restrict t, Transform const *restrict t2)
{
    //calculating distance with pythagoreas
    return sqrtf(pow(t->pos[0] - t2->pos[0], 2) + pow(t->pos[1] - t2->pos[1], 2));
}