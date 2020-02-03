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


void test(void)
{
	//printf("task\n");
}

void test2(void)
{

}

void systest(ComponentTable* c)
{
	//printf("system\n");
}


#define PositionComponent 1
typedef struct{}Position;


void stateMachine_setup(void)
{
	systemManager_init(1, 0, 0, 1);
	entityManager_init(512);
	componentManager_init(0);


	systemManager_system_register(systest, UPDATE, Position, Position);
	systemManager_task_register(test, DRAW);


	stateMachine_state_push(MainMenuState, mainMenuState_construct);
}


#endif