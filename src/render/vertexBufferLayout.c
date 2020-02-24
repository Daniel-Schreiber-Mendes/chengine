#include "../../engine.h"


/*A vertex has many attributes and each attribute has its own layout. 
A VertexBufferLayoutElement is the layout of one attribute of a vbo. A VertexBufferLayout is all its layouts combined.*/


void vertexBufferLayout_construct(VertexBufferLayout *const vbl, uint8_t const size)
{
	vbl->elements = (VertexBufferLayoutElement*)malloc(sizeof(VertexBufferLayoutElement) * size);
	vbl->elementCount = 0;
	vbl->stride = 0;
}


void vertexBufferLayout_destruct(VertexBufferLayout const *const vbl)
{
	free(vbl->elements);
}


void vertexBufferLayout_element_add(VertexBufferLayout *const vbl, VertexBufferLayoutElement const element)
{
	vbl->elements[vbl->elementCount] = element;
	vbl->stride += typeGL_size_get(element.type) * element.componentCount;
	++vbl->elementCount;
	//stride is the size of a vertex. If we add a new attribute to the vertex the size of it is getting bigger
	//element.size is the number of components this attribute has. E.g a 3-dimensional position attribute would have the size 3
}