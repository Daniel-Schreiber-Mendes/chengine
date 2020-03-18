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
		STATIC,
		DYNAMIC
	}
	behaviourType;
}
Collidable;


#define PHYSICS_SYSTEM_COMPONENTS Velocity, Collidable, Transform
void physics_system(checs_system_parameters);
void physics_system_init(void);
void physics_system_terminate(void);

#endif