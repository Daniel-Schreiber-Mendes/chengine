#ifndef UTILITY
#define UTILITY
#include "../resources/resources.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <cglm/cglm.h>
#include <limits.h>


typedef struct
{
	uint16_t x, y;
}vec2ui16;


typedef struct 
{
    uint8_t val, min, max;
    int16_t d;
}Alternator;


typedef struct 
{
	GLint componentCount;
	GLenum type;
	GLboolean normalized;
}VertexBufferLayoutElement;


typedef struct
{
	VertexBufferLayoutElement* elements;
	uint8_t elementCount;
	GLsizei stride;
}VertexBufferLayout;


typedef struct
{
	GLuint id;
	GLsizei indexCount;
}IndexBuffer;


//utility.c
uint8_t  getRandElement(uint8_t const *const array, uint8_t const size);
uint8_t  alternator_alternate(Alternator *const alt);
uint8_t  typeGL_size_get(GLuint const type);

//math.c
uint8_t  limitUint8(uint8_t const val, uint8_t const min, uint8_t const max);
float    limitF(float const val, float const min, float const max);
uint16_t getRandUint16(uint16_t const min, uint16_t const max);
uint8_t  getRandUint8(uint8_t const min, uint8_t const max);
bool     getRandBool(float const chance);
int      roundToMultiple(int const number, int const multiple);
float    getRandF(float min, float max);


//vertexBuffer.c
void	vertexBuffer_construct(GLuint *const vbo, GLsizeiptr const size, void const *const data);
void 	vertexBuffer_destruct(GLuint const *const vbo);
void 	vertexBuffer_bind(GLuint const vbo);

//indexBuffer.c
void	indexBuffer_construct(IndexBuffer *const ibo, GLsizeiptr const size, GLsizei const indexCount, void const *const data);
void 	indexBuffer_destruct(IndexBuffer const *const ibo);
void 	indexBuffer_bind(IndexBuffer const *const ibo);

//vertexArray.c
void	vertexArray_construct(GLuint *const vao);
void 	vertexArray_destruct(GLuint const *const vao);
void 	vertexArray_bind(GLuint const vao);
void    vertexArray_buffer_add(GLuint const vao, GLuint const vbo, VertexBufferLayout const layout);

//program.c
void    program_create(GLuint *const program, char const *const fsPath, char const *const vsPath);
void    program_destroy(GLuint const program);
void 	program_bind(GLuint const program);
void    program_uniform4f_set(GLuint const program, char const *const name, GLfloat const v0, GLfloat const v1, GLfloat const v2, GLfloat const v3);
void    program_uniform1i_set(GLuint const program, char const *const name, GLint const v0);
void    program_uniformMat4f_set(GLuint const program, char const *const name, mat4 const m0);


//vertexBufferLayout.c
void    vertexBufferLayout_construct(VertexBufferLayout *const vbl, uint8_t const size);
void    vertexBufferLayout_destruct(VertexBufferLayout const *const vbl);
void    vertexBufferLayout_element_add(VertexBufferLayout *const vbl, VertexBufferLayoutElement const element);

//renderer.c
void    renderer_draw(GLuint const vao, IndexBuffer const *const ibo, GLuint const program);

#endif