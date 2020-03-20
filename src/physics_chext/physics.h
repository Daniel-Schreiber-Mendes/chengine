#ifndef PHYSICS_H
#define PHYSICS_H
#include "../../engine.h"


typedef struct
{
	union
	{
		float r; //radius if circle
		vec2 bb; //bounding box if rect
	};
	enum
	{
		CIRCLE,
		RECT
	}
	type;
	enum
	{
		STATIC, //has collision enabled, is NOT movable, therefore it shall not have a velocuty component
		KINEMATIC, //has collision and is movable but can not move or interact with other objects despite collision
		DYNAMIC //collision, optional gravity, movable, can interact with other objects
	}
	behaviourType;
}
Collidable;


#define PHYSICS_SYSTEM_COMPONENTS Velocity, Collidable, Transform
void physics_system(checs_system_parameters);
void physics_system_init(void);
void physics_system_terminate(void);

#endif