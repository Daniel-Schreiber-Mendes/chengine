#include "physics.h"


void physics_system(checs_system_parameters)
{
	checs_component_use(Transform, t);
	checs_component_use(Velocity, v);
	checs_component_use(Collidable, c);
}