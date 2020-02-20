#ifndef RENDER_H
#define RENDER_H
#include "../resources/resources.h"
#include "../utility/utility.h"
#include <string.h>
#include <cglm/cglm.h>
#include <checs/checs.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui/cimgui.h>


typedef struct 
{
	GLint componentCount;
	GLenum type;
	GLboolean normalized;
}
VertexBufferLayoutElement;


typedef struct
{
	VertexBufferLayoutElement* elements;
	uint8_t elementCount;
	GLsizei stride;
}
VertexBufferLayout;


typedef struct
{
	GLuint id;
	GLsizei elementCount;
}
ElementBuffer;


typedef struct
{
	vec3 position;	
	mat4 viewm, projm, vpm;
	float rotation;
}
Camera;



#define RenderableComponent 0
typedef struct
{
	GLuint vao, program;
	ElementBuffer ebo;
}
Renderable;


//vertexBuffer.c
void	vertexBuffer_construct(GLuint *const vbo, GLsizeiptr const size, void const *const data);
void 	vertexBuffer_destruct(GLuint const *const vbo);
void 	vertexBuffer_bind(GLuint const vbo);

//elementBuffer.c
void	elementBuffer_construct(ElementBuffer *const ebo, GLsizeiptr const size, GLsizei const elementCount, void const *const data);
void 	elementBuffer_destruct(ElementBuffer const *const ebo);
void 	elementBuffer_bind(ElementBuffer const *const ebo);

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


//camera.c
void camera_construct(Camera *const c, float const left, float const right, float const bottom, float const top);
void camera_vpm_recalculate(Camera *const c);


//render_system.c
#define RENDER_SYSTEM_COMPONENTS Renderable
void render_system(checs_system_parameters);


#endif