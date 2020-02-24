#include "../../engine.h"


void elementBuffer_construct(ElementBuffer *const ebo, GLsizeiptr const size, GLsizei const elementCount, void const *const data)
{
	glGenBuffers(1, &ebo->id);
	elementBuffer_bind(ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	ebo->elementCount = elementCount;
}


void elementBuffer_destruct(ElementBuffer const *const ebo)
{
	glDeleteBuffers(1, &ebo->id);
}


void elementBuffer_bind(ElementBuffer const *const ebo)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id);
}
