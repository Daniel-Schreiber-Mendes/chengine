#ifndef PHYSICS_H
#define PHYSICS_H
#include <chengine-dev/engine.h>
#include "../std_movement/movement.h"


typedef struct
{
	float penetration;
	vec2 normal; //the direction of the collision. for example if the top of a rect collides with another one, the normal is (0, 1)
	vec2 position;
}
Contact;


typedef struct
{
	float friction;
	float mass;
	union
	{
		float r; //radius if circle
		vec2 bb; //bounding box if rect
	};
	enum
	{
		CIRCLE,
		AABB, //axis aligned rectangle, e.g. no rotation
		RECTANGLE, //rectangle with rotaion
		LINE, //line with no ends,
		LINE_SEGMENT, //line with two end
		RAY, //line with one end
		COLLIDABLE_TYPES_COUNT
	}
	type;
	enum
	{
		STATIC, //has collision enabled, is NOT movable, therefore it shall not have a velocity component
		KINEMATIC, //has collision and is movable but can not move or interact with other objects despite collision
		DYNAMIC //collision, optional gravity, movable, can interact with other objects
	}
	behaviourType;
}
Collidable;


typedef struct {}
Gravitatable;


#define sign(x) ((x > 0) - (x < 0))


/* Requirements:
- If a collidable is of type Static, it is not required to have a Velocity component.
- If a collidable is of type Kinematic or Dynamic, they have to have a Velocity component.
- Has to run after the movement task
*/
void physics_task(void);
void physics_gravity_set(vec2 const g);

#endif