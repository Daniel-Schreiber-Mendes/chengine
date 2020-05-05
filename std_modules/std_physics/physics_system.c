#include "physics.h"

/* if a collidable has the type DYNAMIC and it has moved in the last frame then it is not only a collidable but a collider. */
static vec2 gravity;
static Contact contact; //buffer for the last contact made between to colliders


void physics_task(void)
{
	checs_component_use(Transform, t0);
	checs_component_use(Velocity, v0);
	checs_component_use(Collidable, c0);
	checs_component_use(Transform, t1);
	checs_component_use(Velocity, v1);
	checs_component_use(Collidable, c1);


	checs_components_foreach(Collidable, c0, collider)
	{
		checs_component_get(Collidable, c0, collider);

		if (c0->behaviourType != DYNAMIC) continue;

		checs_component_get(Velocity, v0, collider);

		if (v0->vel[0] == 0.0 && v0->vel[1] == 0.0) continue;
		{
			checs_components_foreach(Collidable, c1, collidable)
			{
				if (collidable == collider) continue;

				checs_component_get(Transform, t0, collider);
				checs_component_get(Transform, t1, collidable);

				if (c0->type == AABB && c1->type == AABB)
				{
					if (physics_aabb_aabb_collision(t0, t1, c0->bb, c1->bb))
					{
						goto collision_resolve;
					}
				}
				else if (c0->type == CIRCLE && c1->type == CIRCLE)
				{
					if (physics_circle_circle_collision(t0, t1, c0->r, c1->r))
					{
						goto collision_resolve;
					}
				}

				continue;
				collision_resolve:
					if (c1->behaviourType == DYNAMIC)
					{
						checs_component_get(Velocity, v1, collidable)
						physics_collision_resolve_dynamic(v0, v1, t0, t1);
					}
					else
					{
						physics_collision_resolve_static(v0, t0);
					}
			}
		}
	}

	checs_component_entity_foreach(Gravitatable, gravitatable)
	{
		checs_component_get(Velocity, v0, gravitatable);
		glm_vec2_add(v0->vel, gravity, v0->vel);
	}
}


void physics_gravity_set(vec2 const g)
{
	glm_vec2_copy(g, gravity);
}


bool physics_aabb_aabb_collision(Transform const *restrict t0, Transform const *restrict t1, vec2 const bb0, vec2 const bb1)
{
	//translation vetcor between the middles of the two rects
	vec2 const t = {(t0->pos[0] + bb0[0] * 0.5) - (t1->pos[0] + bb1[0] * 0.5), (t0->pos[1] - bb0[1] * 0.5) - (t1->pos[1] - bb1[1] * 0.5)};
	vec2 const p = {bb0[0] * 0.5 + bb1[0] * 0.5 - fabs(t[0]), bb0[1] * 0.5 + bb1[1] * 0.5 - fabs(t[1])}; //penetration depth on both axes
    if (p[0] > 0 && p[1] > 0)
    {
	    if (p[0] < p[1]) //if collision is mainly on x axis
	    {
	    	contact.penetration = p[0];
	    	contact.normal[0] = -1 * sign(t[0]);
	    	contact.normal[1] = 0;
	    }
	    else //collision is mainly on y axis
	    {
	    	contact.penetration = p[1];
	    	contact.normal[0] = 0;
	    	contact.normal[1] = -1 * sign(t[1]);
	    }
	    contact.position[0] = fmaxf(t0->pos[0], t1->pos[0]);
	    contact.position[1] = fmaxf(t0->pos[1] - bb0[1], t1->pos[1] - bb1[1]);
	    return true;
    }
    return false;
}


void physics_collision_resolve_dynamic(Velocity *restrict v0, Velocity *restrict v1, Transform *restrict t0, Transform *restrict t1)
{
	vec2 impulse;
	{
		vec2 rv;  //relative velocity
		glm_vec2_sub(v1 ? v1 : (vec2){0, 0}, v0, rv);
		float const contactv = glm_vec2_dot(rv, contact.normal);
		if (contactv > 0) return;  //this means objects are moving away from each other
		glm_vec2_scale(contact.normal, -1 * contactv, impulse);
	}
	vec2 impulse0;
	vec2 impulse1;
	glm_vec2_sub(v0->vel, impulse, v0->vel);
	//glm_vec2_sub(v1->vel, impulse, v1->vel);

	// because of floating point errors a small amount of energy constantly gets lost. if an object would stand on top of another, this would result in it slowly sinking
	//inside the other one. because of this we have to correct it a bit by moving the object manually.
	t0->pos[0] -= fmaxf(contact.penetration - 0.001, 0.0) * contact.normal[0] * 0.5;
	t0->pos[1] -= fmaxf(contact.penetration - 0.001, 0.0) * contact.normal[1] * 0.5;
}


void physics_collision_resolve_static(Velocity *const v0, Transform *const t0)
{
	float const contactv = glm_vec2_dot((vec2){-v0->vel[0], -v0->vel[1]}, contact.normal);
	if (contactv > 0) return;
	vec2 impulse;
	glm_vec2_scale(contact.normal, -contactv, impulse);
	glm_vec2_sub(v0->vel, impulse, v0->vel);
	t0->pos[0] -= fmaxf(contact.penetration - 0.001, 0.0) * contact.normal[0] * 0.5;
	t0->pos[1] -= fmaxf(contact.penetration - 0.001, 0.0) * contact.normal[1] * 0.5;
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
	//translation vector between two circle middles
	vec2 const t = {(t1->pos[0] + r1) - (t0->pos[0] + r0), (t1->pos[1] - r1) - (t0->pos[1] - r0)};
	if ((contact.penetration = r0 + r1 - glm_vec2_distance((vec2){0, 0}, t)) > 0) //the penetration is the sum of the radiuses minus the acutal distance
	{
		glm_vec2_normalize_to(t, contact.normal); //the normal is the normalized translation vector between the two circles
		glm_vec2_copy(t0->pos, contact.position);
		glm_vec2_muladds(contact.normal, r0, contact.position); //multiplies vec2 normal and scalar radius and adds them to the middle position of circle 0
	    return true;
	}
	return false;
}