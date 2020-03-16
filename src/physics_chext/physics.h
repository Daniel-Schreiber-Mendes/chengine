#ifndef PHYSICS_H
#define PHYSICS_H
#include "../../engine.h"


typedef struct
{
	float width, height;
}
Collidable;



#define PHYSICS_SYSTEM_COMPONENTS Velocity, Collidable, Transform
void physics_system(checs_system_parameters);


#endif