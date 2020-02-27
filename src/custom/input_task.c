#include "custom.h"


void input_task(void)
{	
	checs_component_use(Transform, t);

	checs_components_foreach(KeyInput, k, entity)
	{
		checs_component_get(Transform, t, entity);
		checs_events_poll(KeyEventData, KeyEvent, event)
		{
			switch (event->key)
			{
				case GLFW_KEY_A:
					t->position[0] -= 0.1;
					break;

				case GLFW_KEY_D:
					t->position[0] += 0.1;
					break;

				case GLFW_KEY_S:
					t->rotation += 0.1f;
					break;

				case GLFW_KEY_W:
					t->rotation -= 0.1f;
					break;
			}
		}
	}
}