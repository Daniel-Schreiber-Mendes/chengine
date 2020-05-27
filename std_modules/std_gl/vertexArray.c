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


void vertexArray_vbl_add(VertexArray *const vao, VertexBufferLayout const layout)
{
	glBindVertexArray(vao->id);
	void const *offset = 0;
	//should be of type uint but glVertexAttribPointer takes a void* where the adress is used as the actual value
	for (uint8_t i=0 ; i < layout.elementCount; ++i)
	{
		vertexBuffer_bind(layout.elements[i].vbo);
		glEnableVertexAttribArray(i + vao->attributeCount);
		glVertexAttribDivisor(i + vao->attributeCount, layout.elements[i].divisor);
		glVertexAttribPointer
		(
			i  + vao->attributeCount, //the index of this attribute
			layout.elements[i].componentCount,   // e.g how many floats per position atribute
			layout.elements[i].type, //e.g GL_FLOAT or GL_UINT
			layout.elements[i].normalized,
			layout.stride,    //the stride is the same for all attributes because it is the size of the whole vertex
			offset // at which offset this attribute begins relative to the begin of the vertex
		);
		offset += layout.elements[i].size * layout.elements[i].componentCount;
	}
	vao->attributeCount += layout.elementCount;
}
