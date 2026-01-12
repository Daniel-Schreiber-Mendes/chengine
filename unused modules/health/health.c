#include "health.h"


void health_import(void)
{
	checs_component_register(<Health>, Health, 200, NULL, NULL);
    checs_component_register(<Damage>, Damage, 1, NULL, NULL);
    checs_tasks_register(CHECS_ON_UPDATE, health_task);
    checs_event_register(<E:DeathEvent>);
}


void health_task(void)
{
	Health *h;

	checs_components_foreach(<Health>, h, entity, ({
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
	checs_command_use(CollisionCommand, cc);
	if (checs_entity_has_component(<Damage>, cc->collider) && checs_entity_has_component(<Health>, cc->collidable))
	{
		checs_component_get_t(<Health>, Health, cc->collidable)->health += checs_component_get_t(<Damage>, Damage, cc->collider)->damage;
	}
	else if (checs_entity_has_component(<Health>, cc->collider) && checs_entity_has_component(<Damage>, cc->collidable))
	{
		checs_component_get_t(<Health>, Health, cc->collider)->health += checs_component_get_t(<Damage>, Damage, cc->collidable)->damage;
	}
}