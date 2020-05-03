#include "physics.h"

/* if a collidable has the type DYNAMIC and it has moved in the last frame then it is not only a collidable but a collider. */
static Vector colliders;
static vec2 gravity = {0, -0.008};
static Contact contact; //buffer for the last contact made between to colliders


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
				if (c0->type == AABB)
				{
					checs_components_foreach(Collidable, c1, collidable)
					{
						if (collidable != collider && c1->type == AABB)
						{
							checs_component_get(Transform, t1, collidable);
							if (physics_aabb_aabb_collision(t0, t1, c0->bb, c1->bb))
							{
								physics_aabb_aabb_single_bump(t0, t1, c0->bb, c1->bb);
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
							if (physics_circle_circle_collision(t0, t1, c0->r, c1->r))
							{
								t0->pos[1] -= v0->vel[1];
							}
							if (physics_circle_circle_collision(t0, t1, c0->r, c1->r))
							{
								t0->pos[0] -= v0->vel[0];
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


void physics_gravity_set(vec2 const g)
{
	gravity[0] = g[0];
	gravity[1] = g[1];
}


bool physics_aabb_aabb_collision(Transform const *restrict t0, Transform const *restrict t1, vec2 const bb0, vec2 const bb1)
{
	vec2 const mid0 = {t0->pos[0] + bb0[0] * 0.5, t0->pos[1] - bb0[1] * 0.5};
	vec2 const mid1 = {t1->pos[0] + bb1[0] * 0.5, t1->pos[1] - bb1[1] * 0.5};
	float const dmidx = mid0[0] - mid1[0];
	float const dmidy = mid0[1] - mid1[1];
    float const px = bb0[0] * 0.5 + bb1[0] * 0.5 - fabs(dmidx); //penetration depth on the x axis
    float const py = bb0[1] * 0.5 + bb1[1] * 0.5 - fabs(dmidy); //penetration depth on the y axis
    if (px > 0 && py > 0)
    {
    	vec2 const normal = {px < py ? -1 * sign(dmidx) : 0, px < py ? 0 : -1 * sign(dmidy)};
    	contact = (Contact)
    	{
    		fminf(px, py), //the penetration depth is defined as the axis of least penetration
    		{normal[0], normal[1]},
    		{mid0[0] + (bb0[0] * 0.5 - 1) * normal[0], 0}
    	}; 
    	return true;
    }
    return false;
}


//this pushes only the first entity out of the second
void physics_aabb_aabb_single_bump(Transform *restrict t0, Transform *restrict t1, vec2 const bb0, vec2 const bb1)
{
    float const right =  (t0->pos[0] + bb0[0]) - t1->pos[0];
    float const left =   (t1->pos[0] + bb1[0]) - t0->pos[0];
    float const bottom = (t0->pos[1] + bb0[1]) - t1->pos[1];
    float const top =    (t1->pos[1] + bb1[1]) - t0->pos[1];
    
    if (right < left && right < top && right < bottom)
    {
        t0->pos[0] = t1->pos[0] - bb0[0];//right collision
    }
    else if (left < top && left < bottom)
    {
        t0->pos[0] = t1->pos[0] + bb1[0];//left collision
    }
    else if (top < bottom)
    {
        t0->pos[1] = t1->pos[1] + bb1[1];//top collision
    }
    else
    {
        t0->pos[1] = t1->pos[1] - bb0[1];//bottom collision
    }
/*
  // Calculate relative velocity
  vec2 rv;
  glm_vec2_sub();
   = B.velocity - A.velocity
 
  // Calculate relative velocity in terms of the normal direction
  float velAlongNormal = DotProduct( rv, normal )
 
  // Do not resolve if velocities are separating
  if(velAlongNormal > 0)
    return;
 
  // Calculate restitution
  float e = min( A.restitution, B.restitution)
 
  // Calculate impulse scalar
  float j = -(1 + e) * velAlongNormal
  j /= 1 / A.mass + 1 / B.mass

    // Apply impulse
    Vec2 impulse = j * normal
    A.velocity -= 1 / A.mass * impulse
	B.velocity += 1 / B.mass * impulse*/
}


//this pushes both entitys by the same amount away from each other
void physics_aabb_aabb_equal_bump(Transform *restrict t0, Transform *restrict t1, vec2 const bb0, vec2 const bb1)
{
    //TODO: implement it :)
}


bool physics_aabb_point_collision(Transform const *restrict t0, Transform const *restrict t1, vec2 const bb0)
{
    return t1->pos[0] > t0->pos[0] && t1->pos[0] < t0->pos[0] + bb0[0] && t1->pos[1] > t0->pos[1] && t1->pos[1] < t0->pos[1] + bb0[1];
}


bool physics_circle_point_collision(Transform const *restrict t0, Transform const *restrict t1, float const r0)
{
    return transform_distance_get(t0, t1) < r0;
}


bool physics_circle_circle_collision(Transform const *restrict t0, Transform const *restrict t1, float const r0, float const r1)
{
    return pow(r0 + r1, 2) > (pow(t0->pos[0] + r0 - t1->pos[0] - r1, 2) + pow(t0->pos[1] + r0 - t1->pos[1] - r1, 2));
    //if
}