#include "../../engine.h"


void vertexBuffer_construct(GLuint *const vbo, GLsizeiptr const size, void const *const data)
{
	glGenBuffers(1, vbo);
	vertexBuffer_bind(*vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}


void vertexBuffer_destruct(GLuint const *const vbo)
{
	glDeleteBuffers(1, vbo);
}


void vertexBuffer_bind(GLuint const vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}
