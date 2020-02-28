#ifndef CUSTOM_H
#define CUSTOM_H
#include "../../engine.h"

typedef struct
{
	State b;
	Texture tex;
}
MainMenuState;


void mainMenuState_construct(MainMenuState *const s);
void mainMenuState_destruct(State *const state);
void mainMenuState_update(State *const state);
void mainMenuState_draw(State *const state);


void input_task(void);


#endif