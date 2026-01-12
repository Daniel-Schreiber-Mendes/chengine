#include "physics.h"

/* if a collidable has the type DYNAMIC and it has moved in the last frame then it is not only a collidable but a collider. */
static vec2 gravity;

static Partition partitions[PARTITION_COUNT][PARTITION_COUNT];
static vec2 partitions_center = {0, 0};

static bool aabb_aabb_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1, Contact *contact);
static bool aabb_circle_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1, Contact *contact);
static bool circle_circle_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1, Contact *contact);
static bool circle_aabb_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1, Contact *contact);
static bool(*collision_tests[2][2]) (Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1, Contact *contact) = 
		{
			{circle_circle_collision, circle_aabb_collision},
			{aabb_circle_collision  , aabb_aabb_collision}
		};


void physics_import(void)
{
	checs_component_register(7, sizeof(Collidable), NULL, NULL, 1 << 1);
	checs_tag_register(1);
	checs_command_register(1);
	checs_tasks_register(CHECS_ON_UPDATE, physics_task, 2);
}


void physics_gravity_import(void)
{
	checs_tasks_register(CHECS_ON_UPDATE, physics_gravity_task, 1);
}

//partition index from position
static inline int16_t p_index_x(float pos)
{
	return (pos - partitions_center[0]) / PARTITION_SIZE + PARTITION_COUNT / 2 - 1;
}

static inline int16_t p_index_y(float pos)
{
	return (pos - partitions_center[1]) / PARTITION_SIZE + PARTITION_COUNT / 2 - 1;
}


static inline int16_t out_of_bounds(int16_t x, int16_t y)
{
	return x >= PARTITION_COUNT || x < 0 || y >= PARTITION_COUNT || y < 0;
}


void physics_task(void)
{
	EntityId e;
	for (uint32_t i=0; checs_components_iterate(7, &e, i); ++i)
	{
		//add entity to all neighbouring partitions if it overlaps
		Transform const *const t = checs_component_get(1, e); //TODO: solve bug when accessing component
		Collidable const *const c = checs_component_get(7, e);
		int16_t x0, x1, y0, y1;
		if (c->type == CIRCLE)
		{
			//add a bit of tolerance so entity doesnt get added if it is only very slightly in neighbouring partition
			x0 = p_index_x(t->pos[0] - c->r + MIN_PENETRATION); 
			x1 = p_index_x(t->pos[0] + c->r - MIN_PENETRATION);
			y0 = p_index_y(t->pos[1] - c->r + MIN_PENETRATION);
			y1 = p_index_y(t->pos[1] + c->r - MIN_PENETRATION);
		}
		else if (c->type == AABB)
		{
			x0 = p_index_x(t->pos[0] - c->bb[0] + MIN_PENETRATION);
			x1 = p_index_x(t->pos[0] + c->bb[0] - MIN_PENETRATION);
			y0 = p_index_y(t->pos[1] - c->bb[1] + MIN_PENETRATION);
			y1 = p_index_y(t->pos[1] + c->bb[1] - MIN_PENETRATION);
		}
		else
		{
			assert(false);
		}

		if (out_of_bounds(x0, y0) || out_of_bounds(x1, y1)) 
		{
/*			printf("delete: ");
			checs_entity_erase(e);
			printf("%u\n", e);
			break;*/
			continue;
		}

		for (int16_t xi=x0; xi<=x1; ++xi)
		{
			for (int16_t yi=y0; yi<=y1; ++yi)
			{
				if (c->behaviourType == DYNAMIC)
				{
					partitions[xi][yi].entitys[partitions[xi][yi].size++] = e;
					assert(partitions[xi][yi].size < ENTITY_PER_PARTITION);
				}
				else if (c->behaviourType == STATIC)
				{
					partitions[xi][yi].entitys_static[partitions[xi][yi].static_size++] = e;
					assert(partitions[xi][yi].static_size < ENTITY_PER_PARTITION);
				}
			}
		}
	}

					/*
				tests every entity against every untested entity:
				test every dynamic entity against every dynamic entity. then test every dynamic entity against every static entity

				D0 D1 S0 D2 S1
				-> generate arrays:
				D0 D1 D2 (dynamic)
				S0 S1 (static)
				
				->test
				D0 vs D1 D2
				D0 vs S0 S1
				D1 vs D2
				D1 vs S0 S1
				D2 vs S0 S1	
				*/
	for (uintEC xi=0; xi < PARTITION_COUNT; ++xi)
	{
		for (uintEC yi=0; yi < PARTITION_COUNT; ++yi)
		{
			Partition *p = &partitions[xi][yi];
			for (uintEC i=0; i < p->size; ++i)
			{
				EntityId collider = p->entitys[i];
				Collidable const *const c0 = checs_component_get(7, collider);
				Transform *const t0 = checs_component_get(1, collider);

				//DYNAMIC VS DYNAMIC
				Contact contact;

				for (uintEC j = 0 + i + 1; j < p->size; ++j)
				{
					EntityId collidable = p->entitys[j];

					Collidable const *const c1 = checs_component_get(7, collidable);
					Transform const *const t1 = checs_component_get(1, collidable);

					if (collision_tests[c0->type][c1->type](t0, t1, c0, c1, &contact))
					{
						Velocity const *const v0 = checs_component_get(2, collider);
						Velocity const *const v1 = checs_component_get(2, collidable);

						vec2 rv;  //relative velocity
						glm_vec2_sub(v1, v0, rv);

						float contactv = glm_vec2_dot(rv, contact.normal);
						if (contactv > 0) continue;  //this means objects are moving away from each other

						vec2 impulse;
						glm_vec2_scale(contact.normal, -contactv, impulse);

						float const c1relMass = c1->mass / (c0->mass + c1->mass);
						float const c0relMass = 1 - c1relMass;

						glm_vec2_muladds(impulse, -c1relMass, v0->vel);
						glm_vec2_muladds(impulse, c0relMass, v1->vel); 

						float correction = contact.penetration * 1.8;
						glm_vec2_muladds(contact.normal, -correction * c0relMass, t0->pos);
						glm_vec2_muladds(contact.normal,  correction * c1relMass, t1->pos);
					}
				}

				//DYNAMIC VS STATIC
				for (uintEC j = 0; j < p->static_size; ++j)
				{
					EntityId collidable = p->entitys_static[j];

					Collidable const *const c1 = checs_component_get(7, collidable);
					Transform const *const t1 = checs_component_get(1, collidable);

					if (collision_tests[c0->type][c1->type](t0, t1, c0, c1, &contact))
					{
						Velocity *const v0 = checs_component_get(2, collider);

						if (!v0->vel[0] && !v0->vel[1]) //entity spawned inside static entity without velocity
						{
							t0->pos[1] += 0.05f;
							continue;
						}

						float contactv = glm_vec2_dot((vec2){-v0->vel[0], -v0->vel[1]}, contact.normal);
						
						if (contactv > 0) continue; //entity is moving away from static entitx
						
						vec2 impulse;
						glm_vec2_scale(contact.normal, -contactv, impulse);
						
						glm_vec2_sub(v0->vel, impulse, v0->vel);

						glm_vec2_muladds(contact.normal, -contact.penetration * 1.8, t0->pos); //correction to prevent sinking
					}
				}
			}
			p->size = 0;
			p->static_size = 0;
		}
	}
}


void physics_partitions_center(vec2 pos)
{
	glm_vec2_copy(pos, partitions_center);
}


void physics_gravity_task(void)
{
	if (!gravity[0] && !gravity[1]) return;

	Velocity *v0;
	EntityId e;
	for (uintEC i=0; e = checs_tags_iterate(1, i); ++i)
	{
		v0 = checs_component_get(2, e);
		glm_vec2_add(v0->vel, gravity, v0->vel);
	}
}


void physics_gravity_set(vec2 const g)
{
	glm_vec2_copy(g, gravity);
}


static bool aabb_aabb_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1, Contact *contact)
{
	//translation vetcor between the middles of the two rects
	vec2 t;
	glm_vec2_sub(t1, t0, t);

	vec2 const p = {c0->bb[0] + c1->bb[0] - fabsf(t[0]), c0->bb[1] + c1->bb[1] - fabsf(t[1])}; //penetration depth on both axesd
	
    if (p[0] < MIN_PENETRATION || p[1] < MIN_PENETRATION) return false; 

    if (p[0] < p[1]) //if collision is mainly on x axis
    {
    	contact->penetration = p[0]; 	
    	contact->normal[0] = sign(t[0]);
    	contact->normal[1] = 0;
    }
    else //collision is mainly on y axis
    {
    	contact->penetration = p[1];
    	contact->normal[0] = 0;
    	contact->normal[1] = sign(t[1]);
    }
    return true;
}


static bool aabb_circle_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1, Contact *contact)
{
	//return circle_aabb_collision(t1, t0, c1, c0);
	return false;
}


static bool circle_circle_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1, Contact *contact)
{

	contact->penetration = c0->r + c1->r - glm_vec2_distance(t0, t1);

	if (contact->penetration < 0) return false;  //the penetration is the sum of the radiuses minus the acutal distance

	//translation vector between two circle middles (distance between middles)
	vec2 t;
	glm_vec2_sub(t1, t0, t);
	glm_vec2_normalize_to(t, contact->normal); //the normal is the normalized translation vector between the two circles
    return true;
}


static bool circle_aabb_collision(Transform const *restrict t0, Transform const *restrict t1, Collidable const *restrict c0, Collidable const *restrict c1, Contact *contact)
{
	vec2 contactPosition = {clamp(t0->pos[0], t1->pos[0] - c1->bb[0], t1->pos[0] + c1->bb[0]), clamp(t0->pos[1], t1->pos[1] - c1->bb[1], t1->pos[1] + c1->bb[1])};
 
    bool const inside = glm_vec2_eqv(t0->pos, contactPosition); //if clamping doesn't change the position, both are the same

    contact->penetration = c0->r - glm_vec2_distance(contactPosition, t0->pos);

	if(contact->penetration < 0 && !inside) return false;

 	glm_vec2_sub(t0->pos, contactPosition, contact->normal);
 	glm_vec2_normalize(contact->normal);

	glm_vec2_negate(contact->normal);

	return true;
}
