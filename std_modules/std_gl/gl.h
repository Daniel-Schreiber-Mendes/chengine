#ifndef GL_EXT_H
#define GL_EXT_H
#include <chengine-dev/engine.h>


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


typedef struct
{
	int width, height, channels;
	GLuint id;
	GLenum unit;
}
Texture;


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

void 	elementBuffer_destruct(ElementBuffer const *ebo);
void 	elementBuffer_bind(ElementBuffer const *ebo);

//vertexArray.c
void vertexArray_construct(VertexArray *vao);
void vertexArray_destruct(VertexArray const *vao);
void vertexArray_bind(VertexArray const *vao);
void vertexArray_buffer_add(VertexArray const *vao, GLuint vbo, VertexBufferLayout layout);
void vertexArray_current_buffer_add(GLuint vbo, VertexBufferLayout layout);

//program.c
void program_construct(Program *program, char const *fsPath, char const *vsPath);
void program_destruct(Program program);
void program_bind(Program program);
void program_uniform4f_set(Program program, char const *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void program_uniform1i_set(Program program, char const *name, GLint v0);
void program_uniformMat4_set(Program program, char const *name, mat4 const m0);
void program_uniform4fv_set(Program program, char const *name, vec4 const v);
void program_uniform1f_set(Program program, char const *name, float v0);
void program_uniform1u_set(Program program, char const *name, float v0);
void program_uniform2uv_set(Program program, char const *name, uint32_t const *v0);
void program_uniform2fv_set(Program program, char const *name, vec2 const v0);


//vertexBufferLayout.c
void vertexBufferLayout_construct(VertexBufferLayout *vbl, uint8_t size);
void vertexBufferLayout_element_add(VertexBufferLayout *vbl, VertexBufferLayoutElement element);
void vertexBufferLayout_destruct(VertexBufferLayout const *vbl);


void texture_construct_from_file(Texture *t, char const *path);
void texture_construct(Texture *t, uint16_t width, uint16_t height);
void texture_update_from_buffer(Texture *t, void* buffer);
void texture_rect_update_from_buffer(Texture *t, uint16_t xoffset, uint16_t yoffset, uint16_t width, uint16_t height, void *buffer);
void texture_destruct(Texture const* t);
void texture_bind(Texture const *t);


#endif