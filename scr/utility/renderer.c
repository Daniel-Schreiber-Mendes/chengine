#include "utility.h"


void renderer_draw(GLuint const vao, IndexBuffer const *const ibo, GLuint const program)
{
	vertexArray_bind(vao);
	indexBuffer_bind(ibo);
	program_bind(program);
	glDrawElements(GL_TRIANGLES, ibo->indexCount, GL_UNSIGNED_INT, NULL);
}