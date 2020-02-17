#ifndef SYSTEMS_H
#define SYSTEMS_H
#include <checs/checs.h>
#include "../components/components.h"

#define RENDER_SYSTEM_COMPONENTS Renderable
static void render_system(checs_system_parameters)
{
	checs_component_use(Renderable, r);

	glClear(GL_COLOR_BUFFER_BIT);

	checs_entity_foreach(entity)
	{
		checs_component_get(Renderable, r, entity);

		vertexArray_bind(r->vao);
		indexBuffer_bind(&r->ibo);
		program_bind(r->program);
		glDrawElements(GL_TRIANGLES, r->ibo.indexCount, GL_UNSIGNED_INT, NULL);
	}
}

#endif