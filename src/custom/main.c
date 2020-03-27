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


int main(void)
{
	checs_init
	(
		1, 					 //systemUpdateCount
		1, 					 //systemDrawCount,
		3, 					 //taskUpdateCount
		0, 					 //taskDrawCount
		1, 					 //tag_count
		ComponentCount, 	 //componentCount
		19, 				 //maxEntitysHint
		StandardCommandCount,//commandCount
		StandardEventCount   //eventCount
	);
	checs_command_subscribe(WindowCloseCommand, window_close_command_callback);
	checs_command_subscribe(FramebufferSizeCommand, framebuffer_size_command_callback);
	checs_command_subscribe(KeyCommand, key_command_callback);
	checs_command_subscribe(MouseButtonCommand, mouse_button_command_callback);
	checs_command_subscribe(JoystickCommand, joystick_command_callback);
	checs_command_subscribe(ScrollCommand, scroll_command_callback);
	checs_component_register(Renderable, 18, renderable_destruct);
	checs_component_register(Transform, 19, NULL);
	checs_component_register(KeyInput, 1, NULL);
	checs_component_register(SoundSource, 1, soundSource_destruct);
	checs_component_register(Camera, 1, NULL);
	checs_component_register(Chunk, 16, NULL);
	checs_component_register(Velocity, 3, NULL);
	checs_component_register(Collidable, 2, NULL);
	checs_event_register(KeyEventData, KeyEvent, 4);
	checs_event_register(MouseButtonEventData, MouseButtonEvent, 1);
	checs_event_register(JoystickEventData, JoystickEvent, 1);
	checs_system_register(render_system, CHECS_ON_DRAW, 1, 5, RENDER_SYSTEM_COMPONENTS);
	checs_system_register(chunk_loading_system, CHECS_ON_UPDATE, 1, 5, CHUNK_LOADING_SYSTEM_COMPONENTS);
	checs_task_register(physics_task, CHECS_ON_UPDATE);
	checs_task_register(input_task, CHECS_ON_UPDATE);
	checs_task_register(movement_task, CHECS_ON_UPDATE);

	audio_init();
    window_init();
    config_load();
    render_system_init();
    physics_task_init();

	texture_construct_from_file(&cobble, "../resources/textures/circles.png");
	texture_construct_from_file(&tileset, "../resources/textures/tilemap.png");
	chunk_loading_system_init(chunk_load_callback, chunk_unload_callback, 2, 1, 32, &tileset, 32);

	stateMachine_init();
	stateMachine_state_push(MainMenuState, mainMenuState_construct);
    stateMachine_run();
    stateMachine_terminate();

	chunk_loading_system_terminate();
	texture_destruct(&cobble);
	texture_destruct(&tileset);
	
    physics_task_terminate();
    render_system_terminate();
    window_terminate();
    audio_terminate();

    checs_terminate();

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