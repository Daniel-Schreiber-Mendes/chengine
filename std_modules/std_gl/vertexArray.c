#include "gl.h"


void vertexArray_construct(VertexArray *const vao)
{
	glGenVertexArrays(1, &vao->id);
	glBindVertexArray(vao->id);
}


void vertexArray_destruct(VertexArray const *const vao)
{
	glDeleteVertexArrays(1, &vao->id);
}


void vertexArray_bind(VertexArray const *const vao)
{
	glBindVertexArray(vao->id);
}


void vertexArray_buffer_add(VertexArray const *const vao, GLuint const vbo, VertexBufferLayout const layout)
{
	glBindVertexArray(vao->id);
	vertexArray_current_buffer_add(vbo, layout);
}


void vertexArray_current_buffer_add(GLuint const vbo, VertexBufferLayout const layout)
{
	vertexBuffer_bind(vbo);
	//each layout of the attributes a the vbo is getting enabled and specified.
	//should be of type uint but glVertexAttribPointer takes a void* where the adress is used as the actual value
	void const *offset = 0;
	for (uint8_t i=0; i < layout.elementCount; ++i)
	{
		VertexBufferLayoutElement const element = layout.elements[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer
		(
			i, 			//the index of this attribute
			element.componentCount,   // e.g how many floats per position atribute
			element.type, //e.g GL_FLOAT or GL_UINT
			element.normalized,
			layout.stride,    //the stride is the same for all attributes because it is the size of the whole vertex
			offset // at which offset this attribute begins relative to the begin of the vertex
		);

		//with each attribute the offset is getting higher 
		offset += getGlTypeSize(element.type) * element.componentCount;
	}
}
