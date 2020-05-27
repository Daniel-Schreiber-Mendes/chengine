#include "gl.h"


void vertexBuffer_construct(VertexBuffer *vbo, void *data, uint16_t size, GLenum type)
{
	glGenBuffers(1, vbo);
	vertexBuffer_bind(*vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, type);
}


void vertexBuffer_destruct(VertexBuffer const *const vbo)
{
	glDeleteBuffers(1, vbo);
}


void vertexBuffer_bind(VertexBuffer const vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}


/*A vertex has many attributes and each attribute has its own layout. 
A VertexBufferLayoutElement is the layout of one attribute of a vbo. A VertexBufferLayout is all its layouts combined.*/


void vertexBufferLayout_construct(VertexBufferLayout *const vbl, uint8_t const size)
{
	vbl->elements = (VertexBufferLayoutElement*)che_malloc(sizeof(VertexBufferLayoutElement) * size);
	vbl->elementCount = 0;
	vbl->stride = 0;
}


void vertexBufferLayout_element_add(VertexBufferLayout *const vbl, VertexBufferLayoutElement const element)
{
	vbl->elements[vbl->elementCount] = element;
	//printf("%lu, %u\n", vbl->elements[vbl->elementCount].size, vbl->elements[vbl->elementCount].type);
	vbl->stride += element.size * element.componentCount;
	++vbl->elementCount;
	//stride is the size of a vertex. If we add a new attribute to the vertex the size of it is getting bigger
	//element.size is the number of components this attribute has. E.g a 3-dimensional position attribute would have the size 3
}


void vertexBufferLayout_destruct(VertexBufferLayout const *const vbl)
{
	che_free(vbl->elements);
}


void elementBuffer_destruct(ElementBuffer const *const ebo)
{
	glDeleteBuffers(1, &ebo->id);
}


void elementBuffer_bind(ElementBuffer const *const ebo)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id);
}


void uniformBuffer_construct(UniformBuffer *const ubo, uint16_t const size, GLenum const type, Program const program, char const *const name, uint8_t const binding)
{

	glGenBuffers(1, ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, *ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, type);

	uint32_t const ubi = glGetUniformBlockIndex(program, name);
	checs_assert(ubi != GL_INVALID_INDEX);

	glUniformBlockBinding(program, ubi, binding);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, *ubo);
}


void uniformBuffer_update(UniformBuffer const ubo, uint16_t const size, void const *const data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	void *const dest = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(dest, data, size);
	glUnmapBuffer(GL_UNIFORM_BUFFER);
}


void uniformBuffer_destruct(UniformBuffer const *const ubo)
{
	glDeleteBuffers(1, ubo);
}


void uniformBuffer_bind(UniformBuffer const ubo)
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
}


void shaderStorageBuffer_construct(ShaderStorageBuffer *const ssbo, uint16_t const size, uint8_t const binding)
{
	glGenBuffers(1, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, *ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_STREAM_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, *ssbo);
}


void shaderStorageBuffer_destruct(ShaderStorageBuffer const *const ssbo)
{
	glDeleteBuffers(1, ssbo);
}


void shaderStorageBuffer_update(ShaderStorageBuffer const ssbo, uint16_t const size, void const *const data)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	void *const dest = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	memcpy(dest, data, size);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}