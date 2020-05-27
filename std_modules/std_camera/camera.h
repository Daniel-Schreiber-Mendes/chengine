#ifndef CAMERA_H
#define CAMERA_H
#include <chengine-dev/engine.h>
#include "../std_movement/movement.h"


typedef struct
{
	float aspectRatio, zoom, rot;
	Transform *target; //this is optionally. if it is null, don't follow anything and just wait for a direct command to move the camera
}
Camera;


#define CameraTag 0


void camera_construct(Camera *c);
void camera_zoom(Camera *c, float zoom);
void camera_resize(Camera *c, uint16_t width, uint16_t height);
void camera_target_set(Camera *c, Transform *t);
void camera_vp_recalculate(Camera *c, mat4 vp);


#endif