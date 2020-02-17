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
	printf("%i, %i\n", sizeData->width, sizeData->height);
}


void key_command_callback(checs_command_parameters)
{
	#define keyData ((KeyData*)data)
	printf("%i\n", keyData->key);
}


void mouse_button_command_callback(checs_command_parameters)
{
	#define buttonData ((MouseButtonData*)data)
	printf("%i\n", buttonData->button);
}

int main(void)
{
	systemManager_init(0, 1, 0, 0);
	entityManager_init();
	componentManager_init(1, 1, 5);
	commandManager_init(COMMAND_COUNT);
	eventManager_init(0);

	checs_command_subscribe(WindowCloseCommand, window_close_command_callback);
	checs_command_subscribe(FramebufferSizeCommand, framebuffer_size_command_callback);
	checs_command_subscribe(KeyCommand, key_command_callback);
	checs_command_subscribe(MouseButtonCommand, mouse_button_command_callback);

	checs_system_register(render_system, ON_DRAW, 1, 5, RENDER_SYSTEM_COMPONENTS);
	checs_component_register(Renderable, 1, 5);

	/*
	checs_event_register(ExitEvent, 2);
	checs_component_register(Position, 1000, 5);

	checs_system_register(sys, ON_UPDATE, 1000, 5, Position);

	for(uintEC i=0; i < 1000; ++i)
	{
		checs_entity_generate(Position);
	}
	
	void* c = malloc(sizeof(WindowResizeCommandData));
	((WindowResizeCommandData*)c)->x = 5;

	checs_command_subscribe(WindowResizeCommand, foo);
	checs_command_publish(WindowResizeCommand, c);
	checs_command_publish(WindowResizeCommand, c);
	checs_command_publish(WindowResizeCommand, c);

	free(c);

	void* event = malloc(sizeof(ExitEventData));
	((ExitEventData*)event)->x = 10;
	checs_event_publish(ExitEvent, event);
	checs_event_publish(ExitEvent, event);

	checs_events_poll(ExitEventData, ExitEvent, event)
	{
		printf("%i\n", ++event->x);
	}

	checs_eventBuffers_swap();

	checs_events_poll(ExitEventData, ExitEvent, event)
	{
		printf("%i\n", ++event->x);
	}*/

	stateMachine_init();
	stateMachine_state_push(MainMenuState, mainMenuState_construct);
    stateMachine_run();

    return 0;
}