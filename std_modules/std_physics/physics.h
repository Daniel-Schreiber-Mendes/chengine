#ifndef PHYSICS_H
#define PHYSICS_H
#include <chengine-dev/engine.h>
#include "../std_movement/movement.h"


typedef struct
{
	float penetration;
	vec2 normal;
	vec2 position;
}
Contact;


typedef struct
{
	union
	{
		float r; //radius if circle
		vec2 bb; //bounding box if rect
		float normal; //vector perpendicular to a line
	};
	enum
	{
		CIRCLE,
		AABB, //axis aligned rectangle, e.g. no rotation
		RECTANGLE, //rectangle with rotaion
		LINE, //line with no ends,
		LINE_SEGMENT, //line with two end
		RAY //line with one end
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

/* Requirements:
- If a collidable is of type Static, it is not required to have a Velocity component.
- If a collidable is of type Kinematic or Dynamic, they have to have a Velocity component.
- Has to run after the movement task
*/
void physics_task(void);
void physics_task_init(void);
void physics_task_terminate(void);
void physics_gravity_set(vec2 const g);
bool physics_aabb_aabb_collision(Transform const *restrict t0, Transform const *restrict t1, vec2 const bb0, vec2 const bb1);
void physics_aabb_aabb_single_bump(Transform *restrict t0, Transform *restrict t1, vec2 const bb0, vec2 const bb1);
void physics_aabb_aabb_equal_bump(Transform *restrict t0, Transform *restrict t1, vec2 const bb0, vec2 const bb1);
bool physics_aabb_point_collision(Transform const *restrict t0, Transform const *restrict t1, vec2 const bb0);
bool physics_circle_point_collision(Transform const *restrict t0, Transform const *restrict t1, float const r0);
bool physics_circle_circle_collision(Transform const *restrict t0, Transform const *restrict t1, float const r0, float const r1);

#endif