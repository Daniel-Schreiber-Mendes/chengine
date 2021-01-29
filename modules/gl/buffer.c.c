#include "gl.h"


void vbo_construct(Vbo *vbo, void const *data, uint16_t size, GLenum type)
{
	glGenBuffers(1, vbo);
	vbo_bind(*vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, type);
}


void vbo_destruct(Vbo const *const vbo)
{
	glDeleteBuffers(1, vbo);
}


void vbo_bind(Vbo const vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}


void vbl_construct(Vbl *const vbl, uint8_t const size)
{
	vbl->elements = (Vble*)che_malloc(sizeof(Vble) * size);
	vbl->elementCount = 0;
	vbl->stride = 0;
}


void vbl_element_add(Vbl *const vbl, Vble const element)
{
	vbl->elements[vbl->elementCount] = element;
	vbl->stride += element.size * element.componentCount;
	++vbl->elementCount;
	//stride is the size of a vertex. If we add a new attribute to the vertex the size of it is getting bigger
	//element.size is the number of components this attribute has. E.g a 3-dimensional position attribute would have the size 3
}


void vbl_destruct(Vbl const *const vbl)
{
	che_free(vbl->elements);
}

void ebo_construct(Ebo *ebo, void const *const data, uint16_t const size, GLenum const type, uint16_t const elementCount)
{
	glGenBuffers(1, &ebo->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, type);
	//ebo->elementCount = elementCount;
}


void ebo_destruct(Ebo const *const ebo)
{
	glDeleteBuffers(1, &ebo->id);
}


void ebo_bind(Ebo const *const ebo)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id);
}


void ubo_construct(Ubo *const ubo, uint16_t const size, GLenum const type, Program const program, char const *const name, uint8_t const binding)
{

	glGenBuffers(1, ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, *ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, type);

	uint32_t const ubi = glGetUniformBlockIndex(program, name);
	che_assert(ubi != GL_INVALID_INDEX);

	glUniformBlockBinding(program, ubi, binding);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, *ubo);
}


void ubo_update_direct(Ubo const ubo, uint16_t const size, void const *const data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	void *const dest = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(dest, data, size);
	glUnmapBuffer(GL_UNIFORM_BUFFER);
}


void ubo_destruct(Ubo const *const ubo)
{
	glDeleteBuffers(1, ubo);
}


void ubo_bind(Ubo const ubo)
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
}


void ssbo_construct(Ssbo *const ssbo, uint16_t const size, uint8_t const binding, GLenum const usage)
{
	glGenBuffers(1, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, *ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, usage);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, *ssbo);
}


void ssbo_destruct(Ssbo const *const ssbo)
{
	glDeleteBuffers(1, ssbo);
}
