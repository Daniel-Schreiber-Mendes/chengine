#ifndef ENGINE_H
#define ENGINE_H
#include "./src/utility/utility.h"
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <checs/checs.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Debug Utilitys /////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

#define CHE_MEMLOG
#define CHE_ASSERT

#ifdef CHE_MEMLOG

	#define che_malloc(size)\
	({\
		void* retVal = malloc((size));\
		printf("\033[0;34m");\
		printf("Che-Allocation:     bytes: %.4u | line: %.3i | file: %s\n", (uint16_t)size, __LINE__, __FILE__ );\
		printf("\033[0m");\
		retVal;\
	})


	#define che_calloc(num, size)\
	({\
		void* retVal = calloc((num), (size));\
		printf("\033[0;34m");\
		printf("Che-Allocation:     bytes: %.4u | line: %.3i | file: %s\n", (uint16_t)size, __LINE__, __FILE__ );\
		printf("\033[0m");\
		retVal;\
	})

	//changes font color to green, print message, reset font color
	#define che_free(p)\
	({\
		free((p));\
		printf("\033[0;32m");\
		printf("Che-Deallocation:                 line: %.3i | file: %s\n", __LINE__, __FILE__);\
		printf("\033[0m");\
	})


#else
	#define che_free(p) free(p)
	#define che_malloc(size) malloc(size)
	#define che_calloc(num, size) calloc(num, size)
#endif

#ifdef CHE_ASSERT
	#define che_assert(expr)\
		if (!(expr))\
		{\
			printf("Che-Assertion:   %s failed. Line: %u, File: %s\n", #expr, __LINE__, __FILE__);\
			exit(-1);\
		}
#else
	#define che_assert(expr) (void)0
#endif


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Definions required by vendor ///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


//#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Render-Utility-Types ///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

typedef GLuint VertexBuffer; //only used when creating a renderable and is stored by the vertexarray. the renderable does not store it
typedef GLuint Program;


typedef struct
{
	GLuint id;
}
VertexArray;


typedef struct 
{
	GLint componentCount;
	GLenum type;
	GLboolean normalized;
}
VertexBufferLayoutElement; //only used when creating a renderable and is stored by the vertexarray. the renderable does not store it


typedef struct
{
	VertexBufferLayoutElement* elements;
	uint8_t elementCount;
	GLsizei stride;
}
VertexBufferLayout; //only used when creating a renderable and is stored by the vertexarray. the renderable does not store it


typedef struct
{
	GLuint id;
	GLsizei elementCount;
}ElementBuffer;


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Components /////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


#define StandardComponentCount  6


typedef struct
{
	Program program;
	VertexArray vao;
	GLenum mode; //can be primitve mode like e.g GL_TRIANGLES or GL_TRIANGLE_STRIP
	uint16_t primitiveCount; //only used if renderableType is one of the two instanced types
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


typedef struct
{
	vec3 position;
	float rotation;
}
Transform;


typedef struct
{
	vec3 vel;
}
Velocity;


typedef struct
{ 
	int i;	
}
KeyInput;


typedef struct
{
	uint32_t source;
}
SoundSource;


typedef struct
{
	mat4 vp;
	float aspectRatio;
	float zoom;
	Transform *target; //this is optionally. of it is null, don't follow anything and just wait for a direct command to move the camera
}
Camera;


void renderable_construct(Renderable *const r);
void renderable_destruct(Renderable *const r);

void  transform_construct(Transform *const t);
void  transform_transform_calculate(Transform *const t, mat4 transform);
float transform_distance_get(Transform *restrict t, Transform *restrict t2);

void velocity_construct(Velocity *const v);

void soundSource_construct(SoundSource *const s, char const *const path);
void soundSource_destruct(SoundSource *const s);
void soundSource_sound_play(SoundSource *const s);
void soundSource_position_set(SoundSource *const s, vec3 const pos);

void camera_construct(Camera *const c);
void camera_default_zoom(Camera *const c, float const yoffset);
void camera_default_resize(Camera *const c, uint16_t const width, uint16_t const height);
void camera_target_set(Camera *const c, Transform *const t);
void camera_default_vp_recalculate(Camera *const c);


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// System-Components //////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


#define RENDER_SYSTEM_COMPONENTS Renderable, Transform
#define INPUT_SYSTEM_COMPONENTS  InputReceiver


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Tags ///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


#define CameraTag 0


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Commands ///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


#define WindowCloseCommand     0
#define FramebufferSizeCommand 1
#define KeyCommand 			   2
#define MouseButtonCommand     3
#define JoystickCommand        4
#define ScrollCommand 		   5
#define StandardCommandCount   6


typedef struct
{
	int const width, height;
}
FramebufferSizeData;


typedef struct
{
	int const key, scancode, action, mods;
}
KeyData;


typedef struct
{
	int const button, action, mods;
}
MouseButtonData;


typedef struct
{
	int const jid, event;
}
JoystickData;


typedef struct
{
	double yoffset;
}
ScrollData;



/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Events ///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


#define KeyEvent 		   0
#define MouseButtonEvent   1
#define JoystickEvent      2
#define StandardEventCount 3


typedef struct
{
	int const key, scancode, action, mods;
}
KeyEventData;


typedef struct
{
	int const button, action, mods;
}
MouseButtonEventData;


typedef struct
{
	int const jid, event;
}
JoystickEventData;


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// State-Base-Type  ///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


typedef struct State State;
struct State 
{
	bool drawing, updating;
    void(*destruct)(State *const);
    void(*update)(State *const);
    void(*draw)(State *const);
};


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Resource-Loading-Utility-Types  ////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


typedef struct
{
	int width, height, channels;
	uint8_t *buffer;
	GLuint id;
}
Texture;


typedef struct
{
	void *buffer;
	uint16_t bufferSize; //size in bytes
}
File;



/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Window  ////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


extern GLFWwindow* window;


void window_init(char const* const title, uint16_t const n_width, uint16_t const n_height);
void window_terminate(void);
void window_fullscreen_set(bool const set);
void window_fullscreen_switch(void);
void window_vsync_set(bool const set);


void inputManager_init(void);
void inputManager_inputReceiver_add(EntityId const entity);


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Statemachine  //////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


void  stateMachine_init(void);
void  stateMachine_state_pop(State *const state);
void _stateMachine_state_push(State *const state);
void  stateMachine_run(void);
void  stateMachine_running_set(bool const n_running);


#define stateMachine_state_push(StateType, stateConstructFunction)\
	State *const state = che_malloc(sizeof(StateType));\
	stateConstructFunction((StateType*)state);\
	_stateMachine_state_push(state);


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Resources  /////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


void texture_construct_from_file(Texture *const t, char const *const path);
void texture_construct(Texture *const t, uint16_t const width, uint16_t const height);
void texture_update_from_buffer(Texture *const t, void* buffer);
void texture_rect_update_from_buffer(Texture *const t, uint16_t const xoffset, uint16_t const yoffset, uint16_t const width, uint16_t const height, void *buffer);
void texture_destruct(Texture const* t);
void texture_bind(Texture *const t);


//file.c
void file_load_text(File *const f, char const *const path);
void file_load_binary(File *const f, char const *const path);
void file_destruct(File const *const f);


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Render  ////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


//vertexBuffer.c
#define vertexBuffer_construct(vbo_ptr, array)\
	glGenBuffers(1, (vbo_ptr));\
	vertexBuffer_bind(*(vbo_ptr));\
	glBufferData(GL_ARRAY_BUFFER, sizeof(array), array, GL_STATIC_DRAW);

void 	vertexBuffer_destruct(VertexBuffer const *const vbo);
void 	vertexBuffer_bind(VertexBuffer const vbo);

//elementBuffer.c
//void	elementBuffer_construct(ElementBuffer *const ebo, GLsizeiptr const size, GLsizei const elementCount, void const *const data);


#define elementBuffer_construct(ebo_ptr, array)\
	glGenBuffers(1, &(ebo_ptr)->id);\
	elementBuffer_bind((ebo_ptr));\
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(array), array, GL_STATIC_DRAW);\
	(ebo_ptr)->elementCount = sizeof(array) / sizeof(array[0]);
	//get number of element in array

void 	elementBuffer_destruct(ElementBuffer const *const ebo);
void 	elementBuffer_bind(ElementBuffer const *const ebo);

//vertexArray.c
void	vertexArray_construct(VertexArray *const vao);
void 	vertexArray_destruct(VertexArray const *const vao);
void 	vertexArray_bind(VertexArray const *const vao);
void    vertexArray_buffer_add(VertexArray const *const vao, GLuint const vbo, VertexBufferLayout const layout);

//program.c
void    program_create(Program *const program, char const *const fsPath, char const *const vsPath);
void    program_destroy(Program const program);
void 	program_bind(Program const program);
void    program_uniform4f_set(Program const program, char const *const name, GLfloat const v0, GLfloat const v1, GLfloat const v2, GLfloat const v3);
void    program_uniform1i_set(Program const program, char const *const name, GLint const v0);
void    program_uniformMat4_set(Program const program, char const *const name, mat4 const m0);
void    program_uniform4fv_set(Program const program, char const *const name, vec4 const v);
void    program_uniform1f_set(Program const program, char const *const name, float const v0);
void    program_uniform1u_set(Program const program, char const *const name, float const v0);


//vertexBufferLayout.c
void    vertexBufferLayout_construct(VertexBufferLayout *const vbl, uint8_t const size);
void    vertexBufferLayout_destruct(VertexBufferLayout const *const vbl);
void    vertexBufferLayout_element_add(VertexBufferLayout *const vbl, VertexBufferLayoutElement const element);


//render_system.c
void render_init(void);
void render_system(checs_system_parameters);
void render_system_custom_draw_callback_set(void(*_custom_draw_callback)(void));
void render_system_imm_rectangle_draw(vec4 const color, vec3 position, vec2 const size);


#define mat4_print(m)\
	for(int i=0; i < 4; ++i)\
	{\
		for(int j=0; j < 4; ++j)\
		{\
			printf("%.2f, ", m[i][j]);\
		}\
		printf("\n");\
	}



/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Audio //////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


void     audio_init(void);
void     audio_terminate(void);
bool     is_little_endian(void); //returns if the machine stores the bits in gib endian or little endian order
uint16_t convert_byte2_buffer_to_uint16(uint8_t const *const buffer);
void*  wavFile_load(char const *const path, uint8_t *channels, uint32_t *const sampleRate, uint8_t *const bps, uint32_t *const size);
uint16_t wav_format_get(uint8_t const channels, uint8_t const bps);


#endif