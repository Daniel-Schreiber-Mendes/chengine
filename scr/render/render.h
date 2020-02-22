#ifndef RENDER_H
#define RENDER_H
#include "../resources/resources.h"
#include "../utility/utility.h"
#include "../window/window.h"
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


#define RenderableComponent 0
typedef struct
{
	GLuint vao, program;
	ElementBuffer ebo;
}
Renderable;


#define TransformComponent 1
typedef struct
{
	float rotation;
	vec3 position;
	vec3 scale;
}
Transform;

#define CameraTag 0

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


//render_system.c
#define RENDER_SYSTEM_COMPONENTS Renderable, Transform
void render_system_init(void);
void render_system(checs_system_parameters);


static void mat4_print(mat4 const m)
{
	for(int i=0; i < 4; ++i)
	{
		for(int j=0; j < 4; ++j)
		{
			printf("%.2f, ", m[i][j]);
		}
		printf("\n");
	}
}

#endif