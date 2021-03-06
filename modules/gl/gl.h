#ifndef GL_EXT_H
#define GL_EXT_H
#include <chengine-dev/engine.h>


#define LAYER_COUNT 4
#define LAYER_WIDTH 512
#define LAYER_HEIGHT 512


typedef uint32_t Vbo; //only used when creating a renderable and is stored by the vertexarray. the renderable does not store it
typedef uint32_t Program;
typedef uint32_t Ubo;
typedef uint32_t Ssbo;
typedef uint32_t Tbo;


typedef struct
{
	uint32_t id;
	uint8_t attributeCount;
}
Vao;


typedef struct 
{
	int componentCount;
	uint8_t size;
	GLenum type;
	GLboolean normalized;
	uint8_t divisor;
	Vbo vbo;
}
Vble; //only used when creating a renderable and is stored by the vertexarray


typedef struct
{
	uint8_t elementCount;
	uint16_t stride; //total size of vertex
	Vble *elements;
}
Vbl; //only used when creating a renderable and is stored by the vertexarray


typedef struct
{
	uint32_t id;
	uint16_t elementCount;
}
Ebo;


typedef struct
{
	vec2 offset;
	vec2 size;
	uint8_t layer;
}
Texture; //texture used by renderer and user. all textures are part of exactly one rtexture. 


typedef struct
{
	uint32_t id;
	GLenum type;
}
RTexture; //raw texture, meaning not managed, used by module creators, not users. has no function for loading it from a file

//gets evaluated at compile time
#define getGlTypeFromBo(Type)\
    ({\
        int retval;\
        if (!strcmp(#Type, "Vbo"))\
            retval = GL_ARRAY_BUFFER;\
        else if (!strcmp(#Type, "Ubo"))\
            retval = GL_UNIFORM_BUFFER;\
        else if (!strcmp(#Type, "Ssbo"))\
            retval = GL_SHADER_STORAGE_BUFFER;\
        else\
        	assert(false);\
        retval;\
    })

#define bo_update(Type, bo, PtrType, p, expr)\
	glBindBuffer(getGlTypeFromBo(Type), bo);\
	PtrType *p = glMapBuffer(getGlTypeFromBo(Type), GL_WRITE_ONLY);\
	expr;\
	glUnmapBuffer(getGlTypeFromBo(Type));



void vbo_construct(Vbo *vbo, void const *data, uint16_t size, GLenum type);
void vbo_destruct(Vbo const *vbo);
void vbo_bind(Vbo const vbo);


void 	ebo_construct(Ebo *ebo, void const *data, uint16_t size, GLenum type, uint16_t elementCount);
void 	ebo_destruct(Ebo const *ebo);
void 	ebo_bind(Ebo const *ebo);


void 	ubo_construct(Ubo *ubo, uint16_t size, GLenum type, Program program, char const *name, uint8_t binding);
void 	ubo_destruct(Ubo const *ubo);
void 	ubo_bind(Ubo ubo);
void 	ubo_update_direct(Ubo ubo, uint16_t size, void const *data);


void 	ssbo_construct(Ssbo *ssbo, uint16_t size, uint8_t binding, GLenum usage);
void 	ssbo_destruct(Ssbo const *ssbo);


void vao_construct(Vao *vao);
void vao_destruct(Vao const *vao);
void vao_bind(Vao const *vao);
void vao_vbl_add(Vao *vao, Vbl layout);


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


void vbl_construct(Vbl *vbl, uint8_t size);
void vbl_element_add(Vbl *vbl, Vble element);
void vbl_destruct(Vbl const *vbl);


void 	textureManager_init(void);
void 	textureManager_terminate(void);
void 	textureManager_image_load(char const *path);
uint8_t textureManager_current_layer_get(void);
void 	texture_update_from_buffer(Texture *t, uint16_t xoffset, uint16_t yoffset, uint16_t width, uint16_t height, void const *buffer);
void 	textureManager_texture_bind(void);

#define texture_construct(t, dx, dy, width, height)\
	*(t) = (Texture){{dx / (float)LAYER_WIDTH, dy / (float)LAYER_HEIGHT}, {width / (float)LAYER_WIDTH, height / (float)LAYER_HEIGHT}, textureManager_current_layer_get() - 1}

#define textureManager_construct_textures_from_file(path, expr)\
({\
	textureManager_image_load(path);\
	(expr);\
})

void rtexture_construct(RTexture *rt, GLenum type);
void rtexture_destruct(RTexture *rt);
void rtexture_bind(RTexture const *rt);


#endif