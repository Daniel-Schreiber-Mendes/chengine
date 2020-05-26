#ifndef GL_EXT_H
#define GL_EXT_H
#include <chengine-dev/engine.h>


#define LAYER_COUNT 4
#define LAYER_WIDTH 512
#define LAYER_HEIGHT 512


typedef uint32_t VertexBuffer; //only used when creating a renderable and is stored by the vertexarray. the renderable does not store it
typedef uint32_t Program;
typedef uint32_t UniformBuffer;


typedef struct
{
	uint32_t id;
}
VertexArray;


typedef struct 
{
	int componentCount;
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
	uint32_t id;
	GLsizei elementCount;
}
ElementBuffer;


typedef struct
{
	vec2 offset;
	vec2 size;
	uint8_t layer;
}
Texture;


//buffer.c
#define vertexBuffer_construct(vbo_ptr, array)\
	glGenBuffers(1, (vbo_ptr));\
	vertexBuffer_bind(*(vbo_ptr));\
	glBufferData(GL_ARRAY_BUFFER, sizeof(array), array, GL_STATIC_DRAW);

void 	vertexBuffer_destruct(VertexBuffer const *const vbo);
void 	vertexBuffer_bind(VertexBuffer const vbo);


//buffer.c

#define elementBuffer_construct(ebo_ptr, array)\
	glGenBuffers(1, &(ebo_ptr)->id);\
	elementBuffer_bind((ebo_ptr));\
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(array), array, GL_STATIC_DRAW);\
	(ebo_ptr)->elementCount = sizeof(array) / sizeof(array[0]);
	//get number of element in array

void 	elementBuffer_destruct(ElementBuffer const *ebo);
void 	elementBuffer_bind(ElementBuffer const *ebo);


void uniformBuffer_construct(UniformBuffer *ubo, uint16_t size, GLenum type);
void uniformBuffer_destruct(UniformBuffer const *ubo);
void uniformBuffer_bind(UniformBuffer ubo);


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
void program_uniform1u_set(Program program, char const *name, uint32_t v0);
void program_uniform1uv_set(Program program, char const *name, uint16_t size, uint32_t const *v0);
void program_uniform2uv_set(Program program, char const *name, uint16_t size, uint32_t const *v0);
void program_uniform2fv_set(Program program, char const *name, uint16_t size, vec2 const v0);
void program_uniform1fv_set(Program program, char const *name, uint16_t size, float const *v0);
void program_uniformMat4v_set(Program program, char const *name, mat4 const *mv, uint16_t size);


//vertexBufferLayout.c
void vertexBufferLayout_construct(VertexBufferLayout *vbl, uint8_t size);
void vertexBufferLayout_element_add(VertexBufferLayout *vbl, VertexBufferLayoutElement element);
void vertexBufferLayout_destruct(VertexBufferLayout const *vbl);


//texture.c
//an image is a buffer of colors that can be load from a file
//a texture is a index into an image to indicate where the texture starts and ends
void textureManager_init(void);
void textureManager_terminate(void);
void textureManager_image_load(char const *path);
uint8_t textureManager_current_layer_get(void);


#define texture_construct(t, offset_x, offset_y, width, height)\
({\
	(t)->offset[0] = offset_x / (float)LAYER_WIDTH;\
	(t)->offset[1] = offset_y / (float)LAYER_HEIGHT;\
	(t)->size[0] = width / (float)LAYER_WIDTH;\
	(t)->size[1] = height / (float)LAYER_HEIGHT;\
	(t)->layer = textureManager_current_layer_get() - 1;\
})

#define textureManager_construct_textures_from_file(path, expr)\
({\
	textureManager_image_load(path);\
	(expr);\
})

void texture_rect_update_from_buffer(Texture *t, uint16_t xoffset, uint16_t yoffset, uint16_t width, uint16_t height, void *buffer);


#endif