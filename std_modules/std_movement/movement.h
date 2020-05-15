#ifndef MOVEMENT_EXT_H
#define MOVEMENT_EXT_H
#include <chengine-dev/engine.h>


typedef struct
{
	vec3 pos;
	float rot;
}
Transform;


typedef struct
{
	vec3 vel;
}
Velocity;


void  transform_transform_calculate(Transform const *const t, mat4 transform);
float transform_distance_get(Transform const *restrict t, Transform const *restrict t2);

/*Requirements:
 - If an entity has a Velocity component it has to have a transform component
*/
void movement_task(void);



#endif