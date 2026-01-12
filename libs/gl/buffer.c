#include "gl.h"


void vertexbuffer_layout_load(Vao *vao, uint16_t *size, Vbl *elements_src, uint8_t elementCount, uint8_t divisor)
{
	*size = 0;
	for (uint8_t i=0; i < elementCount; ++i)
	{
		*size += elements_src[i].size * elements_src[i].componentCount;
	} //stride is the size of a vertex. If we add a new attribute to the vertex the size of it is getting bigger
	//element.size is the number of components this attribute has. E.g a 3-dimensional position attribute would have the size 3

    glBindVertexArray(vao->id);
	for (uint8_t i=0, offset=0 ; i < elementCount; ++i)
	{
		glEnableVertexAttribArray(i + vao->attributeCount);
		//divisor says when to update the vertex attribute, 0 means for every vertex, 1 means for every instance
		glVertexAttribDivisor(i + vao->attributeCount, divisor);
		glVertexAttribPointer
		(
			i  + vao->attributeCount, //the index of this attribute
			elements_src[i].componentCount,   // e.g how many floats per position atribute
			elements_src[i].type, //e.g GL_FLOAT or GL_UINT
			elements_src[i].normalized,
			*size,    //the stride is the same for all attributes because it is the size of the whole vertex
			(void*)offset // at which offset this attribute begins relative to the begin of the vertex
		);
		offset += elements_src[i].size * elements_src[i].componentCount;
	}
	vao->attributeCount += elementCount;
	glBindVertexArray(0);
}

void ubo_construct(uint32_t *const ubo, uint16_t const size, GLenum const type, uint32_t const program, char const *const name, uint8_t const binding)
{

	glGenBuffers(1, ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, *ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, type);

	uint32_t const ubi = glGetUniformBlockIndex(program, name);
	assert(ubi != GL_INVALID_INDEX);

	glUniformBlockBinding(program, ubi, binding);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, *ubo);
}