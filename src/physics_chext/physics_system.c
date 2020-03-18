#include "physics.h"

/* if a collidable has the type DYNAMIC and it has moved in the last frame then it is not only a collidable but a collider. */
static Vector colliders;


void physics_system(checs_system_parameters)
{
	checs_component_use(Transform, t0);
	checs_component_use(Velocity, v0);
	checs_component_use(Collidable, c0);
	checs_component_use(Transform, t1);
	checs_component_use(Collidable, c1);


	checs_components_foreach(Collidable, c0, collidable)
	{
		if (c0->behaviourType == DYNAMIC)
		{
			vector_element_push(&colliders, EntityId, collidable);
		}
	}

	vector_foreach(&colliders, EntityId, collider)
	{
		checs_component_get(Collidable, c0, collider);
		checs_component_get(Velocity, v0, collider);
		checs_component_get(Transform, t0, collider);

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
						t0->position[0] -= v0->vel[0];
					}
					if (transform_circle_circle_collision(t0, t1, c0->r, c1->r))
					{
						t0->position[1] -= v0->vel[1];
					}
				}
			}
		}
	}

	vector_clear(&colliders);
}


void physics_system_init(void)
{
	vector_construct(&colliders, sizeof(EntityId));
}


void physics_system_terminate(void)
{
	vector_destruct(&colliders);
}