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
		checs_component_use(Renderable, r);
    	checs_component_get(Renderable, r, 0);

		if (keyData->key == GLFW_KEY_A)
		{
			c.position[0] -= 0.03;
		}

		if (keyData->key == GLFW_KEY_D)
		{
			c.position[0] += 0.03;
		}

		if (keyData->key == GLFW_KEY_S)
		{
			c.rotation += 0.03;
		}

		if (keyData->key == GLFW_KEY_W)
		{
			c.rotation -= 0.03;
		}

		camera_vpm_recalculate(&c);
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
	/*
	  assert(igDebugCheckVersionAndDataLayout(igGetVersion(), sizeof(ImGuiIO), sizeof(ImGuiStyle),
                                          sizeof(ImVec2), sizeof(ImVec4), sizeof(ImDrawVert),
                                          sizeof(ImDrawIdx)));
  printf("CreateContext() - v%s\n", igGetVersion());
  igCreateContext(NULL);
  ImGuiIO *io = igGetIO();

  unsigned char *text_pixels = NULL;
  int text_w, text_h;
  ImFontAtlas_GetTexDataAsRGBA32(io->Fonts, &text_pixels, &text_w, &text_h, NULL);

  for (int n = 0; n < 20; n++) {
    printf("NewFrame() %d\n", n);

    ImVec2 display_size;
    display_size.x = 1920;
    display_size.y = 1080;
    io->DisplaySize = display_size;
    io->DeltaTime = 1.0f / 60.0f;
    igNewFrame();

    static float f = 0.0f;
    igText("Hello World!");
    igSliderFloat("float", &f, 0.0f, 1.0f, "%.3f", 1.0f);
    igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
    igShowDemoWindow(NULL);

    igRender();
  }

  printf("DestroyContext()\n");
  igDestroyContext(NULL);

  return 0;*/


	systemManager_init(0, 1, 0, 0);
	entityManager_init();
	componentManager_init(1, 1, 5);
	commandManager_init(COMMAND_COUNT);
	eventManager_init(0);

	checs_command_subscribe(WindowCloseCommand, window_close_command_callback);
	checs_command_subscribe(FramebufferSizeCommand, framebuffer_size_command_callback);
	checs_command_subscribe(KeyCommand, key_command_callback);
	checs_command_subscribe(MouseButtonCommand, mouse_button_command_callback);
	checs_command_subscribe(JoystickCommand, joystick_command_callback);

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