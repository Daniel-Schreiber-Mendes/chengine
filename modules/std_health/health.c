#include "health.h"


void health_task(void)
{
	checs_component_mut_use(Health, h);

	checs_components_foreach(<Health>, Health, h, entity, ({
		if (h->health <= 0)
		{
			//DeathEvent e = {entity};
			//checs_event_publish(<E:DeathEvent>, &e);
			checs_entity_erase(entity);
		}
	}))
}



void health_collision_callback(checs_command_parameters)
{
	CollisionCommand *cc = data;
	if (checs_entity_has_component(<Damage>, cc->collider) && checs_entity_has_component(<Health>, cc->collidable))
	{
		checs_component_get_once(<Damage>, Damage, d, cc->collider);
		checs_component_get_once(<Health>, Health, h, cc->collidable);
		h->health += d->damage;
	}
	else if (checs_entity_has_component(<Health>, cc->collider) && checs_entity_has_component(<Damage>, cc->collidable))
	{
		checs_component_get_once(<Health>, Health, h, cc->collider);
		checs_component_get_once(<Damage>, Damage, d, cc->collidable);
		h->health += d->damage;
	}
}