#ifndef RENDER_EXT_H
#define RENDER_EXT_H
#include <chengine-dev/engine.h>
#include "../std_movement/movement.h"
#include "../std_camera/camera.h"
#include "../std_window/window.h"
#include "../std_gl/gl.h"


#define RENDER_SYSTEM_COMPONENTS Renderable, Transform


typedef struct
{
	Program program;
	Vbo pv_vbo, pi_vbo; //per vertex and per instance
	Vao vao;
	uint16_t elementCount; //this is not the number of renderables, but the number of indices in the elementbuffer
	GLenum mode;
	Vector entitys;
}
RectBatch;


typedef struct
{
	Texture const* texture;
	enum
	{
		RT_RECT, //rt for renderable type
		RT_CIRCLE,
		RT_QUAD
	} 
	type;
	float rot;
	vec2 scale;
	vec2 offset; //distance between renderable and center before rotation
}
Renderable;


typedef struct
{
	uint16_t nextFrame;
	float fps;
	Texture *frames;
}
Animatable;


void render_system_on_entity_added(EntityId e);
void render_system_init(void);
void render_system(checs_system_parameters);
void render_system_terminate(void);
void render_system_imm_init(void);
void render_system_imm_terminate(void);
void render_system_imm_rectangle_draw(vec4 const color, vec3 pos, vec2 const size);
void render_system_on_imm_draw(void); //Implemented by user


#define mat4_print(m)\
	for(uint8_t i=0; i < 4; ++i)\
	{\
		for(uint8_t j=0; j < 4; ++j)\
		{\
			printf("%.2f, ", m[i][j]);\
		}\
		printf("\n");\
	}

#endif