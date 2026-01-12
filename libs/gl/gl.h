#ifndef GL_EXT_H
#define GL_EXT_H
#include <GL/glew.h>
#include <cglm/cglm.h>
#include <assert.h>
#include <string.h>

#define LAYER_COUNT 8
#define LAYER_WIDTH 512
#define LAYER_HEIGHT 512

#define UNKNOWN_SIZE 0

typedef struct 
{
	int componentCount;
	uint8_t size;
	GLenum type;
	GLboolean normalized;
}
Vbl; //only used when creating a renderable and is stored by the vertexarray


typedef struct
{
	uint32_t id;
	uint8_t attributeCount;
	uint16_t instance_size; //size of data per instance
	uint16_t vertex_size;  //size of data per vertex
}
Vao;


typedef struct
{
	GLuint id;
	uint16_t width, height; //size in pixels
}
Image;


typedef struct
{
	vec2 pos; //coordinates relative to total image size
	vec2 size;
	Image *image;
}
Texture; //points to a image with information about size and position. is like an index into larger image




void vertexbuffer_layout_load(Vao *vao, uint16_t *size, Vbl *elements_src, uint8_t elementCount, uint8_t divisor);
void ubo_construct(uint32_t *ubo, uint16_t size, GLenum type, uint32_t program, char const *name, uint8_t binding);

void program_construct(uint32_t *program, char const *fsPath, char const *vsPath);
void program_uniform4f_set(uint32_t program, char const *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void program_uniform1i_set(uint32_t program, char const *name, GLint v0);
void program_uniformMat4_set(uint32_t program, char const *name, mat4 const m0);
void program_uniform4fv_set(uint32_t program, char const *name, vec4 const v);
void program_uniform1f_set(uint32_t program, char const *name, float v0);
void program_uniform1u_set(uint32_t program, char const *name, uint32_t v0);
void program_uniform1uv_set(uint32_t program, char const *name, uint16_t size, uint32_t const *v0);
void program_uniform2uv_set(uint32_t program, char const *name, uint16_t size, uint32_t const *v0);
void program_uniform2fv_set(uint32_t program, char const *name, uint16_t size, vec2 const v0);
void program_uniform1fv_set(uint32_t program, char const *name, uint16_t size, float const *v0);
void program_uniformMat4v_set(uint32_t program, char const *name, mat4 const *mv, uint16_t size);

void 	textureManager_init(void);
void 	textureManager_terminate(void);
Image 	textureManager_image_load(char const *path);
void 	texture_construct(Texture *const t, uint16_t const dx, uint16_t const dy, uint16_t const width, uint16_t const height, Image *texture_id);


#endif