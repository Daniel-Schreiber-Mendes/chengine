#ifndef CAMERA_H
#define CAMERA_H
#include <chengine-dev/engine.h>
#include "../movement_ext/movement.h"


typedef struct
{
	mat4 vp;
	float aspectRatio;
	float zoom;
	Transform *target; //this is optionally. if it is null, don't follow anything and just wait for a direct command to move the camera
}
Camera;


void camera_construct(Camera *const c);
void camera_default_zoom(Camera *const c, float const yoffset);
void camera_default_resize(Camera *const c, uint16_t const width, uint16_t const height);
void camera_target_set(Camera *const c, Transform *const t);
void camera_default_vp_recalculate(Camera *const c);


#endif