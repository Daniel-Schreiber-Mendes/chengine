#include "state/MainMenuState.h"


void window_close_command_callback(checs_command_parameters)
{
	printf("Window should close\n");
	stateMachine_running_set(false);
}


void framebuffer_size_command_callback(checs_command_parameters)
{
	#define sizeData ((FramebufferSizeData*)data)
	glViewport(0, 0, sizeData->width, sizeData->height);
	
}


void key_command_callback(checs_command_parameters)
{
	#define keyData ((KeyData*)data)
	//printf("%i\n", keyData->key);
	if (keyData->key == GLFW_KEY_F11 && keyData->action == GLFW_PRESS)
	{
		window_fullscreen_switch();
	}
	
	if (keyData->action == GLFW_REPEAT || keyData->action == GLFW_PRESS)
	{
		checs_component_get_once(Transform, t, checs_entity_get_by_tag(CameraTag));

		if (keyData->key == GLFW_KEY_A)
		{
			t->position[0] -= 0.1;
		}

		if (keyData->key == GLFW_KEY_D)
		{
			t->position[0] += 0.1;
		}

		if (keyData->key == GLFW_KEY_S)
		{
			t->rotation += 0.1f;
		}

		if (keyData->key == GLFW_KEY_W)
		{
			t->rotation -= 0.1f;
		}
	}
}


void mouse_button_command_callback(checs_command_parameters)
{
	#define buttonData ((MouseButtonData*)data)
}


void joystick_command_callback(checs_command_parameters)
{
	#define cData ((JoystickData*)data)
	printf("jid:%i, event: %i\n", cData->jid, cData->event);
}

int main(void)
{
	systemManager_init(0, 1, 0, 0);
	entityManager_init();
	componentManager_init(2, 100, 5);
	commandManager_init(COMMAND_COUNT);
	eventManager_init(0);

	checs_command_subscribe(WindowCloseCommand, window_close_command_callback);
	checs_command_subscribe(FramebufferSizeCommand, framebuffer_size_command_callback);
	checs_command_subscribe(KeyCommand, key_command_callback);
	checs_command_subscribe(MouseButtonCommand, mouse_button_command_callback);
	checs_command_subscribe(JoystickCommand, joystick_command_callback);


	checs_component_register(Renderable, 50, 5);
	checs_component_register(Transform, 100, 5);

	checs_system_register(render_system, ON_DRAW, 20, 5, RENDER_SYSTEM_COMPONENTS);


	/*
	checs_event_register(ExitEvent, 2);

	void* event = malloc(sizeof(ExitEventData));
	((ExitEventData*)event)->x = 10;
	checs_event_publish(ExitEvent, event);
	checs_event_publish(ExitEvent, event);

	checs_events_poll(ExitEventData, ExitEvent, event)
	{
		printf("%i\n", ++event->x);
	}
	*/

	stateMachine_init();
	stateMachine_state_push(MainMenuState, mainMenuState_construct);
    stateMachine_run();

    return 0;
}