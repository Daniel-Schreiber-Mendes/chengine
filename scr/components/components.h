#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "../utility/utility.h"

#define RenderableComponent 0
typedef struct
{
	GLuint vao, program;
	IndexBuffer ibo;
}
Renderable;


#endif