#ifndef ENGINE_H
#define ENGINE_H
#include "utility.h"
#include "debug_utils.h"
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <checs/checs.h>
#include <stdbool.h>


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Forward declarations ///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


typedef struct _Texture Texture;
typedef struct _State State;


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
	VertexBufferLayoutElement *elements;
	uint8_t elementCount;
	GLsizei stride;
}
VertexBufferLayout; //only used when creating a renderable and is stored by the vertexarray. the renderable does not store it


typedef struct
{
	GLuint id;
	GLsizei elementCount;
}
ElementBuffer;


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// State-Base-Type  ///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


struct _State 
{
	bool drawing, updating;
    void(*destruct)(State *const);
    void(*update)(State *const);
    void(*draw)(State *const);
};


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Resource-Loading-Utility-Types  ////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


struct _Texture
{
	int width, height, channels;
	GLuint id;
	GLenum unit;
};


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Statemachine  //////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


void  stateMachine_init(void);
void  stateMachine_terminate(void);
void  stateMachine_state_pop(State *const state);
void _stateMachine_state_push(State *const state);
void  stateMachine_run(void);
void  stateMachine_running_set(bool const n_running);
extern void stateMachine_first_state_push(void); //user implemented


#define stateMachine_state_push(Type, stateConstructFunc)\
	State *const state = che_malloc(sizeof(Type));\
	stateConstructFunc((Type*)state);\
	_stateMachine_state_push(state);


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Resources  /////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


void texture_construct_from_file(Texture *const t, char const *const path);
void texture_construct(Texture *const t, uint16_t const width, uint16_t const height);
void texture_update_from_buffer(Texture *const t, void* buffer);
void texture_rect_update_from_buffer(Texture *const t, uint16_t const xoffset, uint16_t const yoffset, uint16_t const width, uint16_t const height, void *buffer);
void texture_destruct(Texture const* t);
void texture_bind(Texture const *const t);


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
void    program_construct(Program *const program, char const *const fsPath, char const *const vsPath);
void    program_destruct(Program const program);
void 	program_bind(Program const program);
void    program_uniform4f_set(Program const program, char const *const name, GLfloat const v0, GLfloat const v1, GLfloat const v2, GLfloat const v3);
void    program_uniform1i_set(Program const program, char const *const name, GLint const v0);
void    program_uniformMat4_set(Program const program, char const *const name, mat4 const m0);
void    program_uniform4fv_set(Program const program, char const *const name, vec4 const v);
void    program_uniform1f_set(Program const program, char const *const name, float const v0);
void    program_uniform1u_set(Program const program, char const *const name, float const v0);


//vertexBufferLayout.c
void vertexBufferLayout_construct(VertexBufferLayout *const vbl, uint8_t const size);
void vertexBufferLayout_element_add(VertexBufferLayout *const vbl, VertexBufferLayoutElement const element);
void vertexBufferLayout_destruct(VertexBufferLayout const *const vbl);


/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// User implemented ///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


extern void che_checs_init(void);
extern void che_init(void);
extern void che_terminate(void);


#endif