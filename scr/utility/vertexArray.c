#include "utility.h"


void vertexArray_construct(GLuint *const vao)
{
	glGenVertexArrays(1, vao);
	vertexArray_bind(*vao);
}


void vertexArray_destruct(GLuint const *const vao)
{
	glDeleteVertexArrays(1, vao);
}


void vertexArray_bind(GLuint const vao)
{
	glBindVertexArray(vao);
}


void vertexArray_buffer_add(GLuint const vao, GLuint const vbo, VertexBufferLayout const layout)
{
	vertexArray_bind(vao);
	vertexBuffer_bind(vbo);
	//each layout of the attributes a the vbo is getting enabled and specified.
	uint16_t offset = 0;
	for (uint8_t i=0; i < layout.elementCount; ++i)
	{
		VertexBufferLayoutElement const element = layout.elements[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer
		(
			i, 			//the index of this attribute
			element.componentCount,   // e.g how many floats per position atribute
			element.type,
			element.normalized,
			layout.stride,    //the stride is the same for all attributes because it is the size of the whole vertex
			(void*)offset // at which offset this attribute begins relative to the begin of the vertex
		);

		//with each attribute the offset is getting higher 
		//printf("\noffset: %u\nstride: %u\ncomponents: %i\n", offset, layout.stride, element.componentCount);
		offset += typeGL_size_get(element.type) * element.componentCount;
	}
}
