#include "custom.h"


void input_task(void)
{	
	checs_component_use(Transform, t);

	componentManager_components_foreach(KeyInput, c, entity)
	{
		checs_component_get(Transform, t, entity);
		checs_events_poll(KeyEventData, KeyEvent, event)
		{
			if (event->key == GLFW_KEY_A)
			{
				t->position[0] -= 0.1;
			}

			if (event->key == GLFW_KEY_D)
			{
				t->position[0] += 0.1;
			}

			if (event->key == GLFW_KEY_S)
			{
				t->rotation += 0.1f;
			}

			if (event->key == GLFW_KEY_W)
			{
				t->rotation -= 0.1f;
			}
		}
	}
}