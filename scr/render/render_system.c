#include "render.h"


static Camera* camera;

void render_system(checs_system_parameters)
{
	checs_component_use(Renderable, r);

	glClear(GL_COLOR_BUFFER_BIT);

	checs_entity_foreach(entity)
	{
		checs_component_get(Renderable, r, entity);

		vertexArray_bind(r->vao);
		program_bind(r->program);

		glDrawElements(GL_TRIANGLES, r->ebo.elementCount, GL_UNSIGNED_INT, NULL);
	}
}