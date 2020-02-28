#include "custom.h"


static void window_close_command_callback(checs_command_parameters);
static void framebuffer_size_command_callback(checs_command_parameters);
static void key_command_callback(checs_command_parameters);
static void mouse_button_command_callback(checs_command_parameters);
static void joystick_command_callback(checs_command_parameters);
static void scroll_command_callback(checs_command_parameters);


int main(void)
{
	glClearColor(1, 0.4, 0.3, 0);
	systemManager_init(0, 1, 1, 0);
	entityManager_init();
	componentManager_init(ComponentCount, 3, 5);
	commandManager_init(CommandCount);
	eventManager_init(EventCount);


	checs_command_subscribe(WindowCloseCommand, window_close_command_callback);
	checs_command_subscribe(FramebufferSizeCommand, framebuffer_size_command_callback);
	checs_command_subscribe(KeyCommand, key_command_callback);
	checs_command_subscribe(MouseButtonCommand, mouse_button_command_callback);
	checs_command_subscribe(JoystickCommand, joystick_command_callback);
	checs_command_subscribe(ScrollCommand, scroll_command_callback);


	checs_component_register(Renderable, 2, 5);
	checs_component_register(Transform, 3, 5);
	checs_component_register(KeyInput, 2, 5);
	checs_component_register(SoundSource, 1, 5);
	checs_component_register(Camera, 1, 5);


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


static void window_close_command_callback(checs_command_parameters)
{
	stateMachine_running_set(false);
}


static void framebuffer_size_command_callback(checs_command_parameters)
{
	#define sizeData ((FramebufferSizeData*)data)
	checs_component_get_once(Camera, c, checs_entity_get_by_tag(CameraTag));
	camera_default_resize(c, sizeData->width, sizeData->height);
}


static void key_command_callback(checs_command_parameters)
{
	#define keyData ((KeyData*)data)
	if (keyData->key == GLFW_KEY_F11 && keyData->action == GLFW_PRESS)
	{
		window_fullscreen_switch();
	}
}


static void mouse_button_command_callback(checs_command_parameters)
{
	#define buttonData ((MouseButtonData*)data)
}


static void joystick_command_callback(checs_command_parameters)
{
	#define cData ((JoystickData*)data)
}


static void scroll_command_callback(checs_command_parameters)
{
	#define offset ((ScrollData*)data)
	checs_component_get_once(Camera, c, checs_entity_get_by_tag(CameraTag));
	camera_default_zoom(c, offset->yoffset);
}