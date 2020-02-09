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
	int x;
	int y;
}Position;

#define TransformComponent 1
typedef struct
{
	int x;
	int y;
}Transform;



void sys(checs_system_parameters)
{
	checs_component_use(Position, p);

	checs_entity_foreach(entity)
	{
		checs_component_get(Position, p, entity);
		++p->x;
		++p->y;
		printf("%i\n", p->x);
	}
}


void stateMachine_setup(void)
{
	checs_init(1, 0, 0, 0, 2, 1, 10);

	checs_components_register(Position, Transform);
	checs_system_register(sys, UPDATE, 1, 50, Position);

	for(uintEC i=0; i < 1; ++i)
	{
		checs_entity_generate(Position);
	}

	stateMachine_state_push(MainMenuState, mainMenuState_construct);
}


#endif