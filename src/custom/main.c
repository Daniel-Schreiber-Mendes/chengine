#include "custom.h"


void window_close_command_callback(checs_command_parameters)
{
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
	if (keyData->key == GLFW_KEY_F11 && keyData->action == GLFW_PRESS)
	{
		window_fullscreen_switch();
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


void scroll_command_callback(checs_command_parameters)
{
	#define offset ((ScrollData*)data)
	checs_component_get_once(Transform, t, checs_entity_get_by_tag(CameraTag));
	/*the offset can be either -1 or 1*/
	if (t->scale < 0.11f && offset->yoffset < 0)
		return;
	t->scale += offset->yoffset * 0.1f;
}


int main(void)
{
	glClearColor(1, 0.4, 0.3, 0);
	systemManager_init(0, 1, 1, 0);
	entityManager_init();
	componentManager_init(ComponentCount, 2, 5);
	commandManager_init(CommandCount);
	eventManager_init(EventCount);


	checs_command_subscribe(WindowCloseCommand, window_close_command_callback);
	checs_command_subscribe(FramebufferSizeCommand, framebuffer_size_command_callback);
	checs_command_subscribe(KeyCommand, key_command_callback);
	checs_command_subscribe(MouseButtonCommand, mouse_button_command_callback);
	checs_command_subscribe(JoystickCommand, joystick_command_callback);
	checs_command_subscribe(ScrollCommand, scroll_command_callback);


	checs_component_register(Renderable, 1, 5);
	checs_component_register(Transform, 2, 5);
	checs_component_register(KeyInput, 2, 5);
	checs_component_register(SoundSource, 1, 5);


	checs_event_register(KeyEventData, KeyEvent, 4);
	checs_event_register(MouseButtonEventData, MouseButtonEvent, 1);
	checs_event_register(JoystickEventData, JoystickEvent, 1);


	checs_system_register(render_system, ON_DRAW, 1, 5, RENDER_SYSTEM_COMPONENTS);


	checs_task_register(input_task, ON_UPDATE);


	stateMachine_init();
	stateMachine_state_push(MainMenuState, mainMenuState_construct);
    stateMachine_run();

    return 0;
}