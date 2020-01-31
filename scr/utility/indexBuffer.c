#include "utility.h"


void indexBuffer_construct(IndexBuffer *const ibo, GLsizeiptr const size, GLsizei const indexCount, void const *const data)
{
	glGenBuffers(1, &ibo->id);
	indexBuffer_bind(ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	ibo->indexCount = indexCount;
}


void indexBuffer_destruct(IndexBuffer const *const ibo)
{
	glDeleteBuffers(1, &ibo->id);
}


void indexBuffer_bind(IndexBuffer const *const ibo)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id);
}
