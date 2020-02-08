#include "states.h"

static Vector states; 
static bool validState = true;
static bool running = true;


void stateMachine_init(void)
{	 
    vector_construct(&states, sizeof(State*)); 
    window_init("");
    stateMachine_setup();
}


void stateMachine_state_pop(State *const state)
{
    vector_element_erase(&states, State*, state);
    state->destruct(state);
    validState = false;
}


void stateMachine_run(void)
{
    
	while(!glfwWindowShouldClose(window))
	{        
        glfwPollEvents();
        
        vector_foreach(&states, State*, state)
        {      
            state->update(state);
            systemManager_systems_call(UPDATE);
            systemManager_tasks_call(UPDATE);
            if (validState) // check if state is valid, in case it got deleted
            {
                state->draw(state);
                systemManager_systems_call(DRAW);
                systemManager_tasks_call(DRAW);
            }
            else // if state isnt valid make it valid again, since the invalid state got deleted
            {
                validState = true;
            }
        }
	}
    stateMachine_terminate();
}


void _stateMachine_state_push(State *const state)
{
    vector_element_push(&states, State*, state);
}


void stateMachine_terminate(void)
{
    window_terminate();
    checs_terminate();
    vector_foreach(&states, State*, state)
    {
        state->destruct(state);
    }
    vector_destruct(&states);
}
