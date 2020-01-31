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
	
}

void stateMachine_setup(void)
{
	systemManager_task_register(test, UPDATE);
	stateMachine_state_push(MainMenuState, mainMenuState_construct);
}



#endif