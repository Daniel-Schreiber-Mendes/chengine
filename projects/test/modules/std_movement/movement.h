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


void  transform_transform_calculate(Transform *const t, mat4 transform);
float transform_distance_get(Transform const *restrict t, Transform const *restrict t2);
bool  transform_circle_circle_collision(Transform const *restrict t0, Transform const *restrict t1, float const r0, float const r1);
bool  transform_circle_point_collision(Transform const *restrict t0, Transform const *restrict t1, float const r0);
bool  transform_rect_rect_collision(Transform const *restrict t0, Transform const *restrict t1, vec2 const bb0, vec2 const bb1);
bool  transform_rect_point_collision(Transform const *restrict t0, Transform const *restrict t1, vec2 const bb0);


/*Requirements:
 - If an entity has a Velocity component it has to have a transform component
*/
void movement_task(void);



#endif