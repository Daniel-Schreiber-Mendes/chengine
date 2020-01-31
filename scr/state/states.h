#ifndef STATES
#define STATES

#include "../window/window.h"
#include <string.h>
#include <checl/containers.h>
#include <checs/ecs.h>


typedef struct State State; //the structs that "inherit" from State have to have it as their FIRST member, otherwise they can't be casted correctly
struct State 
{
	bool drawing, updating;
    void(*destruct)(State *const);
    void(*update)(State *const);
    void(*draw)(State *const);
};


void 		stateMachine_init(void);
void 		stateMachine_state_pop(State *const state);
void 		_stateMachine_state_push(State *const state);
void 		stateMachine_run(void);
void 		stateMachine_terminate(void);
extern void stateMachine_setup(void); //user defined


//StateType is the typename of a type derived from State. stateConstructFunction is the function that gets called 
// to initialize/create/construct the statetype
#define stateMachine_state_push(StateType, stateConstructFunction)\
	StateType *const state = (StateType*)malloc(sizeof(StateType));\
	stateConstructFunction(state);\
	_stateMachine_state_push((State*)state);



#endif