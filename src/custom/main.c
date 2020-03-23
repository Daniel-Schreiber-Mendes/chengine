#include "custom.h"


static Texture tileset;
Texture cobble;


static void window_close_command_callback(checs_command_parameters);
static void framebuffer_size_command_callback(checs_command_parameters);
static void key_command_callback(checs_command_parameters);
static void mouse_button_command_callback(checs_command_parameters);
static void joystick_command_callback(checs_command_parameters);
static void scroll_command_callback(checs_command_parameters);
static void chunk_unload_callback(Chunk const *const c);
static EntityId chunk_load_callback(void);


typedef struct
{
	int x;
}Test;

int main(void)
{
	systemManager_init(1, 1, 3, 0);
	entityManager_init();
	componentManager_init(ComponentCount, 28);
	commandManager_init(StandardCommandCount);
	eventManager_init(StandardEventCount);


	checs_command_subscribe(WindowCloseCommand, window_close_command_callback);
	checs_command_subscribe(FramebufferSizeCommand, framebuffer_size_command_callback);
	checs_command_subscribe(KeyCommand, key_command_callback);
	checs_command_subscribe(MouseButtonCommand, mouse_button_command_callback);
	checs_command_subscribe(JoystickCommand, joystick_command_callback);
	checs_command_subscribe(ScrollCommand, scroll_command_callback);
	

	checs_component_register(Renderable, 27);
	checs_component_register(Transform, 28);
	checs_component_register(KeyInput, 1);
	checs_component_register(SoundSource, 1);
	checs_component_register(Camera, 1);
	checs_component_register(Chunk, 25);
	checs_component_register(Velocity, 3);
	checs_component_register(Collidable, 2);


	checs_event_register(KeyEventData, KeyEvent, 4);
	checs_event_register(MouseButtonEventData, MouseButtonEvent, 1);
	checs_event_register(JoystickEventData, JoystickEvent, 1);


	checs_system_register(render_system, ON_DRAW, 1, 5, RENDER_SYSTEM_COMPONENTS);
	checs_system_register(chunk_loading_system, ON_UPDATE, 1, 5, CHUNK_LOADING_SYSTEM_COMPONENTS);


	checs_task_register(physics_task, ON_UPDATE);
	checs_task_register(input_task, ON_UPDATE);
	checs_task_register(movement_task, ON_UPDATE);

	stateMachine_init();

	texture_construct_from_file(&cobble, "../resources/textures/circles.png");
	texture_construct_from_file(&tileset, "../resources/textures/tilemap.png");
	chunk_loading_system_init(chunk_load_callback, chunk_unload_callback, 2, 1, 32, &tileset, 32);
	physics_task_init();

    window_vsync_set(true);
    window_fullscreen_set(true);
    window_cursor_visible_set(false);

	stateMachine_state_push(MainMenuState, mainMenuState_construct);

    stateMachine_run();
    physics_task_terminate();

    return 0;
}


static void chunk_unload_callback(Chunk const *const c)
{

}


static EntityId chunk_load_callback(void)
{
	EntityId chunk = checs_entity_generate(Chunk, Renderable, Transform);
	chunk_construct(chunk);
	return chunk;
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