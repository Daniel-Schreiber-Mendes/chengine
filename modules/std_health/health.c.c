#include "health.h"


void health_task(void)
{
	checs_component_mut_use(Health, h);

	checs_components_foreach(8, Health, h, entity, ({
		if (h->health <= 0)
		{
			//DeathEvent e = {entity};
			//checs_event_publish(DeathEvent, 3, &e);
			checs_entity_erase(entity);
		}
	}))
}



void health_collision_callback(checs_command_parameters)
{
	CollisionCommand *cc = data;
	if (checs_entity_has_component(9, cc->collider) && checs_entity_has_component(8, cc->collidable))
	{
		checs_component_get_once(9, Damage, d, cc->collider);
		checs_component_get_once(8, Health, h, cc->collidable);
		h->health += d->damage;
	}
	else if (checs_entity_has_component(8, cc->collider) && checs_entity_has_component(9, cc->collidable))
	{
		checs_component_get_once(8, Health, h, cc->collider);
		checs_component_get_once(9, Damage, d, cc->collidable);
		h->health += d->damage;
	}
}