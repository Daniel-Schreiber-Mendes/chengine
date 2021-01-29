#include "movement.h"


void foobar_(void)
{
	<E:X>
}


void foo_bar(void)
{
	<E:X>
}


void foobar(void)
{
	<0>
}
/*	checs_component_register(<Transform>, Transform, 200, NULL, NULL);
	checs_component_register(<Velocity>, Velocity, 200, NULL, NULL);
	checs_tasks_register(CHECS_ON_UPDATE, movement_task);*/

void movement_task(void)
{
	Transform *t;
	Velocity *v;

	checs_components_foreach(<Velocity>, v, e, ({
		t = checs_component_get(<Transform>, e);
		glm_vec2_scale(v->vel, 0.96, v->vel);
		glm_vec2_add(v->vel, t->pos, t->pos);
	}));
}


float transform_distance_get(Transform const *restrict t, Transform const *restrict t2)
{
    return sqrtf(pow(t->pos[0] - t2->pos[0], 2) + pow(t->pos[1] - t2->pos[1], 2));
}
