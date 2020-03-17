#include "physics.h"


void physics_system(checs_system_parameters)
{
	checs_component_use(Transform, t0);
	checs_component_use(Velocity, v0);
	checs_component_use(Collidable, c0);
	checs_component_use(Transform, t1);
	checs_component_use(Collidable, c1);


	checs_entity_foreach(collider)
	{
		checs_component_get(Transform, t0, collider);
		checs_component_get(Collidable, c0, collider);
		checs_component_get(Velocity, v0, collider);

		if (c0->type == CIRCLE)
		{
			checs_entity_foreach(collidable)
			{
				checs_component_get(Collidable, c1, collidable);
				if (collidable != collider && c1->type == CIRCLE)
				{
					checs_component_get(Transform, t1, collidable);
					if (transform_circle_circle_collision(t0, t1, c0->r, c1->r))
					{
						printf("Collision occured! distance: %.1f\n", transform_distance_get(t0, t1));
					}
				}
			}
		}
	}
}