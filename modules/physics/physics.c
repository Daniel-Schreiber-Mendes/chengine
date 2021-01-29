#include "physics.h"

/* if a collidable has the type DYNAMIC and it has moved in the last frame then it is not only a collidable but a collider. */
static vec2 gravity;
static Contact contact; //buffer for the last contact made between to collidables
static bool aabb_aabb_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1);
static bool aabb_circle_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1);
static bool circle_circle_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1);
static bool circle_aabb_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1);
static bool(*collision_tests[2][2]) (Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1) = 
		{
			{circle_circle_collision, circle_aabb_collision},
			{aabb_circle_collision  , aabb_aabb_collision}
		};


void physics_import(void)
{
	checs_component_register(<Collidable>, Collidable, 200, NULL, NULL);
	checs_attribute_register(<A:Gravitatable>, 200);
	checs_commands_register(<C:CollisionCommand>);
	checs_tasks_register(CHECS_ON_UPDATE, physics_task);
}


void physics_gravity_import(void)
{
	checs_attribute_register(<A:Gravitatable>, 200);
	checs_tasks_register(CHECS_ON_UPDATE, physics_gravity_task);
}


void physics_task(void)
{
	Transform *t0;
	Velocity *v0, *v1;
	Collidable const *c0, *c1;
	Transform const *t1;

	checs_components_foreach(<Collidable>, c0, collider, ({
		c0 = checs_component_get(<Collidable>, collider);

		if (c0->behaviourType != DYNAMIC) continue;

		v0 = checs_component_get(<Velocity>, collider);

		{
			checs_components_foreach(<Collidable>, c1, collidable, ({
				if (collidable == collider) continue;
				t0 = checs_component_get(<Transform>, collider);
				t1 = checs_component_get(<Transform>, collidable);


				if (collision_tests[c0->type][c1->type](t0, t1, c0, c1))
				{
					checs_command_publish(<C:CollisionCommand>, (&(CollisionCommand){collider, collidable}));
					vec2 impulse;
					float contactv;
					// because of floating point errors a small amount of energy constantly gets lost. if an object would stand on top of another, this would result in it slowly sinking
					//inside the other one. because of this we have to correct it a bit by moving the object manually.
					float const correction = fmaxf(contact.penetration - 0.001f, 0.f) * 1.2f;

					if (c1->behaviourType == DYNAMIC)
					{
						v1 = checs_component_get(<Velocity>, collidable);

						vec2 rv;  //relative velocity
						glm_vec2_sub(v1 ? v1 : (vec2){0, 0}, v0, rv);
						
						contactv = glm_vec2_dot(rv, contact.normal);
						
						if (contactv > 0) continue;  //this means objects are moving away from each other
						glm_vec2_scale(contact.normal, -contactv, impulse);

						glm_vec2_sub(v0->vel, (vec2){impulse[0] * c1->mass / (c0->mass + c1->mass), impulse[1] * c1->mass / (c0->mass + c1->mass)}, v0->vel);
						glm_vec2_add(v1->vel, (vec2){impulse[0] * c0->mass / (c0->mass + c1->mass), impulse[1] * c0->mass / (c0->mass + c1->mass)}, v1->vel);

						glm_vec2_muladds(contact.normal, -correction * c0->mass / (c0->mass + c1->mass), t0->pos);
						glm_vec2_muladds(contact.normal,  correction * c1->mass / (c0->mass + c1->mass), t1->pos);

					}
					else
					{
						contactv = glm_vec2_dot((vec2){-v0->vel[0], -v0->vel[1]}, contact.normal);
						
						if (contactv > 0) continue;
						glm_vec2_scale(contact.normal, -contactv, impulse);
						
						glm_vec2_sub(v0->vel, impulse, v0->vel);
						glm_vec2_muladds(contact.normal, -correction, t0->pos);
					}
				}
			}))
		}
	}))
}


void physics_gravity_task(void)
{
	Velocity *v0;
	checs_attribute_entity_foreach(<A:Gravitatable>, gravitatable)
	{
		v0 = checs_component_get(<Velocity>, gravitatable);
		glm_vec2_add(v0->vel, gravity, v0->vel);
	}
}


void physics_gravity_set(vec2 const g)
{
	glm_vec2_copy(g, gravity);
}


static bool aabb_aabb_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1)
{
	//translation vetcor between the middles of the two rects
	vec2 t;
	glm_vec2_sub(t1, t0, t);
	vec2 const p = {c0->bb[0] + c1->bb[0] - fabsf(t[0]), c0->bb[1] + c1->bb[1] - fabsf(t[1])}; //penetration depth on both axes
	
    if (p[0] < 0 || p[1] < 0)
    	return false;

    if (p[0] < p[1]) //if collision is mainly on x axis
    {
    	contact.penetration = p[0]; 	
    	contact.normal[0] = sign(t[0]);
    	contact.normal[1] = 0;
    }
    else //collision is mainly on y axis
    {
    	contact.penetration = p[1];
    	contact.normal[0] = 0;
    	contact.normal[1] = sign(t[1]);
    }
    return true;
}


static bool aabb_circle_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1)
{
	return false;
}


static bool circle_circle_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1)
{
	//translation vector between two circle middles
	vec2 t;
	glm_vec2_sub(t1, t0, t);

	if ((contact.penetration = c0->r + c1->r - glm_vec2_distance((vec2){0, 0}, t)) < 0) //the penetration is the sum of the radiuses minus the acutal distance
		return false;

	glm_vec2_normalize_to(t, contact.normal); //the normal is the normalized translation vector between the two circles
    return true;
}


static bool circle_aabb_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1)
{
	vec2 contactPosition = {clamp(t0->pos[0], t1->pos[0] - c1->bb[0], t1->pos[0] + c1->bb[0]), clamp(t0->pos[1], t1->pos[1] - c1->bb[1], t1->pos[1] + c1->bb[1])};
 
    bool const inside = glm_vec2_eqv(t0->pos, contactPosition); //if clamping doesn't change the position, both are the same

	if((contact.penetration = c0->r - glm_vec2_distance(contactPosition, t0->pos)) < 0 && !inside)
    	return false;

 	glm_vec2_sub(t0->pos, contactPosition, contact.normal);
 	glm_vec2_normalize(contact.normal);

	if(!inside)
		glm_vec2_negate(contact.normal);

	return true;
}