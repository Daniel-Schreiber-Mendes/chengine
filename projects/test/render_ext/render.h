#ifndef RENDER_EXT_H
#define RENDER_EXT_H
#include <chengine-dev/engine.h>
#include "../movement_ext/movement.h"
#include "../camera_ext/camera.h"


#define RENDER_SYSTEM_COMPONENTS Renderable, Transform


typedef struct
{
	Program program;
	VertexArray vao;
	GLenum mode; //can be primitve mode like e.g GL_TRIANGLES or GL_TRIANGLE_STRIP
	uint16_t primitiveCount; //only used if renderableType is one of the two instanced types
	Texture const* texture;
	union
	{
		uint16_t vertexCount;
		uint16_t elementCount;
	};
	enum
	{
		ARRAYS,
		ARRAYS_INSTANCED,
		ELEMENTS,
		ELEMENTS_INSTANCED
	}
	renderableType;
}
Renderable;


void renderable_construct(Renderable *const r);
void renderable_destruct(Renderable *const r);


//render_system.c
/* Requirements:
- If a renderable is of type ARRAY it has to have its vertexCount defined
- If a renderable is of type ARRAYS_INSTANCED it has to have its vertexCount and primitiveCount defined
- If a renderable is of type ELEMENTS it has to have its elementCount defined
- If a renderable is of type ELEMENTS_INSTANCED it has to have its elementCount and primitveCount defined
*/
void render_system_init(void);
void render_system(checs_system_parameters);
void render_system_terminate(void);
void render_system_custom_draw_callback_set(void(*_custom_draw_callback)(void));
void render_system_imm_rectangle_draw(vec4 const color, vec3 pos, vec2 const size);


#define mat4_print(m)\
	for(int i=0; i < 4; ++i)\
	{\
		for(int j=0; j < 4; ++j)\
		{\
			printf("%.2f, ", m[i][j]);\
		}\
		printf("\n");\
	}

#endif