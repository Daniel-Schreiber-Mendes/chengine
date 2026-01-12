#ifndef RENDER_EXT_H
#define RENDER_EXT_H
#include "../../libs/gl/gl.h"
#include "../movement/movement.h"
#include "../window/window.h"
#include <cglm/cglm.h>
#include <checs/checs.h>


typedef struct
{
	uint32_t program;
	uint32_t pv_vbo, pi_vbo; //per vertex and per instance
	Vao vao;
	uint32_t ebo;// buffer for vertex indexing
	uint8_t ebo_size; //number of elements in elementbuffer, e.g. 6 for a square because 2 triangles with 3 vertices each
	uint32_t renderableCount;
	GLenum mode;
	Image* image; //3D Texture
	uint32_t camera_ubo;
	EntityId camera;
}
RectBatch;


typedef struct
{
	Texture const* texture;
	enum
	{
		RT_RECT, //rt for renderable type
	} 
	type;
	float rot;
	vec2 scale;
	vec2 offset; //distance between renderable and position(which is the center) before rotation
}
Renderable;


typedef struct
{
	float aspectRatio, zoom, rot, width, height;
	EntityId target; //this is optionally. if it is null, don't follow anything and just wait for a direct command to move the camera
}
Camera;


void render_import(void);
void render_task_init(void);
void render_task_terminate(void);
void render_task(void);
void render_rectBatch_image_set(Image* image);
void render_rectBatch_camera_set(EntityId camera);


void camera_import(void);
void camera_construct(Camera *c);
void camera_zoom(Camera *c, float zoom);
void camera_resize(Camera *c, uint16_t width, uint16_t height);
void camera_target_set(Camera *c, EntityId e);
void camera_vp_recalculate(EntityId id, mat4 vp);



#endif