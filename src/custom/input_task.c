#include "custom.h"


void input_task(void)
{	
	checs_component_use(Velocity, v);


	checs_components_foreach(KeyInput, k, entity)
	{
		checs_component_get(Velocity, v, entity);
		/*checs_events_poll(KeyEventData, KeyEvent, event)
		{
			if(event->action == GLFW_PRESS || event->action == GLFW_REPEAT)
			{
				switch (event->key)
				{
					case GLFW_KEY_W:
						v->vel[1] += 0.008;
						break;

					case GLFW_KEY_A:
						v->vel[0] -= 0.008;
						break;

					case GLFW_KEY_S:
						v->vel[1] -= 0.008;
						break;

					case GLFW_KEY_D:
						v->vel[0] += 0.008;
						break;
				}
			}
		}*/

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			v->vel[1] += 0.1;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			v->vel[0] -= 0.1;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			v->vel[1] -= 0.1;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			v->vel[0] += 0.1;
		}
	}
}