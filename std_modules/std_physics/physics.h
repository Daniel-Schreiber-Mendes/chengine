#ifndef PHYSICS_H
#define PHYSICS_H
#include <chengine-dev/engine.h>
#include "../std_movement/movement.h"


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
		RECTANGLE
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


typedef struct {}
Gravitatable;

/* Requirements:
- If a collidable is of type Static, it is not required to have a Velocity component.
- If a collidable is of type Kinematic or Dynamic, they have to have a Velocity component.
*/
void physics_task(void);
void physics_task_init(void);
void physics_task_terminate(void);

#endif