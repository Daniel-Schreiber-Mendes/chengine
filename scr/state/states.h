#ifndef STATES_H
#define STATES_H
#include "../render/render.h"


void  stateMachine_init(void);
void  stateMachine_state_pop(State *const state);
void _stateMachine_state_push(State *const state);
void  stateMachine_run(void);
void  stateMachine_running_set(bool const n_running);

//StateType is the typename of a type derived from State. stateConstructFunction is the function that gets called 
// to initialize/create/construct the statetype
#define stateMachine_state_push(StateType, stateConstructFunction)\
	State *const state = malloc(sizeof(StateType));\
	stateConstructFunction((StateType*)state);\
	_stateMachine_state_push(state);



#endif