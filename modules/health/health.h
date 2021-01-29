#ifndef STD_HEALTH_MOD_H
#define STD_HEALTH_MOD_H
#include <chengine-dev/engine.h>

#include "../physics/physics.h"

typedef struct
{
	int16_t health;
}
Health; //component


typedef struct
{
	int16_t damage;
	enum
	{
		DT_PLAYER,
		DT_ENEMY,
		DT_NPC
	}
	type;
}
Damage; //component


typedef struct
{
	EntityId entity;
} 
DeathEvent;


void health_import(void);
void health_task(void);
void health_collision_callback(checs_command_parameters);


#endif