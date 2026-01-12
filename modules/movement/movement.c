#include "movement.h"

void movement_import(void)
{
	checs_component_register(<Transform>, sizeof(Transform), NULL, NULL, 0);
	checs_component_register(<Velocity>, sizeof(Velocity), NULL, NULL, 1 << <Transform>);
	checs_component_register(<Rotation>, sizeof(Rotation), NULL, NULL, 0);
	checs_tasks_register(CHECS_ON_UPDATE, movement_task, 1);
}


void movement_task(void)
{
	Transform *t; 
	Velocity *v;
	EntityId e;
	for (uintEC i=0; v = checs_components_foreach(<Velocity>, &e, i); ++i)
	{
		t = checs_component_get(<Transform>, e);
		glm_vec2_scale(v->vel, 0.98, v->vel);
		glm_vec2_add(v->vel, t->pos, t->pos);
	}
}


float transform_distance_get(Transform const *restrict t, Transform const *restrict t2)
{
    return sqrtf(pow(t->pos[0] - t2->pos[0], 2) + pow(t->pos[1] - t2->pos[1], 2));
}