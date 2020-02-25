#ifndef CUSTOM_H
#define CUSTOM_H
#include "../../engine.h"
#include "../../audio.h"

typedef struct
{
	State b;

	GLuint vbo;
	VertexBufferLayout vbl;
	Texture tex;
}
MainMenuState;


void mainMenuState_construct(MainMenuState *const s);
void mainMenuState_destruct(State *const state);
void mainMenuState_update(State *const state);
void mainMenuState_draw(State *const state);


void input_task(void);


#endif