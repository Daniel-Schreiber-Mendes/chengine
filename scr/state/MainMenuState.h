#ifndef MAINMENUSTATE
#define MAINMENUSTATE
#include "states.h"
#include "../utility/utility.h"
#include "../resources/resources.h"


typedef struct
{
	State b;

	GLfloat positions[5 * 4];
	GLuint indices[6];

	GLuint vbo, vao, program;
	VertexBufferLayout vbl;
	IndexBuffer ibo;
	Texture tex;

	mat4 view, model, mvp, proj;

}MainMenuState;


void mainMenuState_construct(MainMenuState *const s);
void mainMenuState_destruct(State *const state);
void mainMenuState_update(State *const state);
void mainMenuState_draw(State *const state);



void task(void)
{
	//printf("task\n");
}


#define PositionComponent 0
typedef struct
{
	char x;
}Position;

#define TransformComponent 1
typedef struct
{
	//int x;
	char x;
}Transform;



void sys(checs_system_parameters)
{
	checs_component_use(Position, p);


	checs_entity_foreach(entity)
	{
		checs_component_get(Position, p, entity);
		//printf("%u\n", entity);

	}

	checs_componentMatches_foreach(entity, Position, Position, Transform)
	{
		printf("hallo%u\n", entity);
	}
}


#define WindowResizeCommand 0
typedef struct
{
	int x;
}WindowResizeCommandData;


#define ExitEvent 0
typedef struct
{
	int x;	
}ExitEventData;


void foo(void* data)
{
	printf("%i\n", ((ExitEventData*)data)->x);
}


void stateMachine_setup(void)
{
	systemManager_init(1, 0, 0, 0);
	entityManager_init();
	componentManager_init(1, 1000, 10);
	commandManager_init(1);
	eventManager_init(1);

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

	checs_events_poll(ExitEventData, ExitEvent, exitEvent)
	{
		printf("%i\n", ++exitEvent->x);
	}

	checs_eventBuffers_swap();

	checs_events_poll(ExitEventData, ExitEvent, exitEvent)
	{
		printf("%i\n", ++exitEvent->x);
	}

	stateMachine_state_push(MainMenuState, mainMenuState_construct);
}

#endif