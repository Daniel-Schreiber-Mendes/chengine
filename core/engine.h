#ifndef ENGINE_H
#define ENGINE_H
#include "utility.h"
#include "debug.h"
#include <cglm/cglm.h>
#include <checs/checs.h>


typedef struct State State;
struct State 
{
    void(*destruct)(State *const);
    void(*update)(State *const);
    void(*draw)(State *const);
	bool drawing, updating;
};


void  stateMachine_state_pop(State *const state);
void _stateMachine_state_push(State *const state);
void  stateMachine_run(void);
void  stateMachine_running_set(bool const set);
extern void stateMachine_first_state_push(void);


#define stateMachine_state_push(Type, stateConstructFunc)\
	State *const state = che_malloc(sizeof(Type));\
	stateConstructFunc((Type*)state);\
	_stateMachine_state_push(state);


extern void che_checs_init(void);
extern void che_init(void);
extern void che_terminate(void);
extern void che_poll_events(void);


#endif