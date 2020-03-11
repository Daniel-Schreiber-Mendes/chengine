#include "custom.h"


static void window_close_command_callback(checs_command_parameters);
static void framebuffer_size_command_callback(checs_command_parameters);
static void key_command_callback(checs_command_parameters);
static void mouse_button_command_callback(checs_command_parameters);
static void joystick_command_callback(checs_command_parameters);
static void scroll_command_callback(checs_command_parameters);
static void chunk_unload_callback(Chunk const *const c);
static void chunk_load_callback(Chunk *const c);


int main(void)
{
	systemManager_init(1, 1, 2, 0);
	entityManager_init();
	componentManager_init(ComponentCount, 3, 5);
	commandManager_init(StandardCommandCount);
	eventManager_init(StandardEventCount);


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
	checs_component_register(Chunk, 1, 5);
	checs_component_register(Velocity, 2, 5);


	checs_event_register(KeyEventData, KeyEvent, 4);
	checs_event_register(MouseButtonEventData, MouseButtonEvent, 1);
	checs_event_register(JoystickEventData, JoystickEvent, 1);


	checs_system_register(render_system, ON_DRAW, 1, 5, RENDER_SYSTEM_COMPONENTS);
	checs_system_register(chunk_loading_system, ON_UPDATE, 1, 5, CHUNK_LOADING_SYSTEM_COMPONENTS);


	checs_task_register(input_task, ON_UPDATE);
	checs_task_register(movement_task, ON_UPDATE);


	chunk_loading_system_init(chunk_load_callback, chunk_unload_callback, 2, 1);


	stateMachine_init();
	stateMachine_state_push(MainMenuState, mainMenuState_construct);
    stateMachine_run();

    return 0;
}


static void chunk_unload_callback(Chunk const *const c)
{
	//printf("chunk unloaded\n");
}


static void chunk_load_callback(Chunk *const c)
{
	//printf("chunk loaded\n");
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