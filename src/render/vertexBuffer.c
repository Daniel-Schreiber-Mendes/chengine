#include "../../engine.h"


void vertexBuffer_destruct(GLuint const *const vbo)
{
	glDeleteBuffers(1, vbo);
}


void vertexBuffer_bind(GLuint const vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}
