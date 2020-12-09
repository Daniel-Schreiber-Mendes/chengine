#ifndef RENDER_EXT_H
#define RENDER_EXT_H
#include <chengine-dev/engine.h>
#include "../std_movement/movement.h"
#include "../std_camera/camera.h"
#include "../std_window/window.h"
#include "../std_gl/gl.h"
#include "../std_text/text.h"


typedef struct
{
	Program program;
	Vbo pv_vbo, pi_vbo; //per vertex and per instance
	Vao vao;
	uint16_t elementCount; //this is not the number of renderables, but the number of indices in the elementbuffer
	GLenum mode;
}
RectBatch;


typedef struct
{
	Texture const* texture;
	enum
	{
		RT_RECT, //rt for renderable type
		RT_QUAD
	} 
	type;
	float rot;
	vec2 scale;
	vec2 offset; //distance between renderable and position(which is the center) before rotation
}
Renderable;


void render_task_init(void);
void render_task_terminate(void);
void render_task(void);
void render_task_camera_set(EntityId c);

void render_task_imm_init(void);
void render_task_imm_terminate(void);
void render_task_imm_rectangle_draw(vec4 const color, vec2 const pos, vec2 const size);
void render_task_imm_text_draw(vec4 const color, vec3 const pos, Text const *t);
void render_task_imm_font_load(Font *font);
void render_task_on_imm_draw(void); //Implemented by user


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