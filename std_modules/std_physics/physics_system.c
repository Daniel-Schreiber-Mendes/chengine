#include "physics.h"

/* if a collidable has the type DYNAMIC and it has moved in the last frame then it is not only a collidable but a collider. */
static Vector colliders;
static float gravity[2] = {0, -0.15};


void physics_task(void)
{
	checs_component_use(Transform, t0);
	checs_component_use(Velocity, v0);
	checs_component_use(Collidable, c0);
	checs_component_use(Transform, t1);
	checs_component_use(Collidable, c1);


	checs_components_foreach(Collidable, c0, collider)
	{
		checs_component_get(Collidable, c0, collider);
		checs_component_get(Transform, t0, collider);

		if (c0->behaviourType == DYNAMIC)
		{
			checs_component_get(Velocity, v0, collider)
			if (v0->vel[0] != 0.0f || v0->vel[1] != 0.0f)
			{
				if (c0->type == RECTANGLE)
				{
					checs_components_foreach(Collidable, c1, collidable)
					{
						if (collidable != collider && c1->type == RECTANGLE)
						{
							checs_component_get(Velocity, v0, collider);
							checs_component_get(Transform, t1, collidable);
							if (transform_rect_rect_collision(t0, t1, c0->bb, c1->bb))
							{
								t0->pos[0] -= v0->vel[0];
							}
							if (transform_rect_rect_collision(t0, t1, c0->bb, c1->bb))
							{
								t0->pos[1] -= v0->vel[1];
							}
						}
					}
				}
				else if (c0->type == CIRCLE)
				{
					checs_components_foreach(Collidable, c1, collidable)
					{
						if (collidable != collider && c1->type == CIRCLE)
						{
							checs_component_get(Velocity, v0, collider);
							checs_component_get(Transform, t1, collidable);
							if (transform_circle_circle_collision(t0, t1, c0->r, c1->r))
							{
								t0->pos[0] -= v0->vel[0];
							}
							if (transform_circle_circle_collision(t0, t1, c0->r, c1->r))
							{
								t0->pos[1] -= v0->vel[1];
							}
						}
					}
				}
			}
		}
	}

	vector_clear(&colliders);

	checs_component_entity_foreach(Gravitatable, gravitatable)
	{
		checs_component_get(Velocity, v0, gravitatable);
		v0->vel[0] += gravity[0];
		v0->vel[1] += gravity[1];
	}
}


void physics_task_init(void)
{
	vector_construct(&colliders, sizeof(EntityId));
}


void physics_task_terminate(void)
{
	vector_destruct(&colliders);
}