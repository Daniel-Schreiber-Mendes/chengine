#include "../../engine.h"


void elementBuffer_destruct(ElementBuffer const *const ebo)
{
	glDeleteBuffers(1, &ebo->id);
}


void elementBuffer_bind(ElementBuffer const *const ebo)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id);
}
