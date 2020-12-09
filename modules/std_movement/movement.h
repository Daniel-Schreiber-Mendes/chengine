#ifndef MOVEMENT_EXT_H
#define MOVEMENT_EXT_H
#include <chengine-dev/engine.h>


typedef struct
{
	vec3 pos;
}
Transform;


typedef struct
{
	vec3 vel;
}
Velocity;


void movement_task(void);
float transform_distance_get(Transform const *restrict t, Transform const *restrict t2);


#endif