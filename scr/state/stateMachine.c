#include "states.h"

static Vector states; 
static bool running = true, validState = true;

void stateMachine_init(void)
{	 
    vector_construct(&states, sizeof(State*)); 
}


void stateMachine_state_pop(State *const state)
{
    vector_element_erase(&states, State*, state);
    state->destruct(state);
    free(state);
    validState = false;
}


void stateMachine_run(void)
{
    double time;
    int frames;
	while(running)
	{                
        ++frames;
        glfwPollEvents();
        checs_eventBuffers_swap();
        vector_foreach(&states, State*, state)
        {      
            state->update(state);
            if (validState) // check if state is valid, in case it got deleted
            {
                state->draw(state);
            }
            else // if state isnt valid make it valid again, since the invalid state got deleted
            {
                validState = true;
            }
        }
	}
    
    //printf("%f\n", (glfwGetTime() - time) / frames);
    window_terminate();
    vector_foreach(&states, State*, state)
    {
        state->destruct(state);
        free(state);
    }
    checs_terminate();
    vector_destruct(&states);
}


void _stateMachine_state_push(State *const state)
{
    vector_element_push(&states, State*, state);
}


void stateMachine_running_set(bool const n_running)
{
    running = n_running;
}