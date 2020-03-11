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
			v->vel[1] += 0.008;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			v->vel[0] -= 0.008;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			v->vel[1] -= 0.008;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			v->vel[0] += 0.008;
		}
	}
}


void movement_task(void)
{
	checs_component_use(Transform, t); //because every entity who has a velocity component also has a transform component, we only need a task, not a system
	checs_components_foreach(Velocity, v, entity)
	{
		checs_component_get(Transform, t, entity);
		glm_vec3_add(v->vel, t->position, t->position); //because we dont want to change the z position use vec2 which only has a x and z component
		v->vel[0] *= 0.75f;
		v->vel[1] *= 0.75f;
	}
}