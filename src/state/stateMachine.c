#include "../../engine.h"

static Vector states; 
static bool running = true, validState = true;

void stateMachine_init(void)
{	 
    vector_construct(&states, sizeof(State*));
    audio_init();
    window_init("Che Engine Window", 640, 480);
    render_init();
}


void stateMachine_state_pop(State *const state)
{
    vector_element_erase(&states, State*, state);
    state->destruct(state);
    che_free(state);
    validState = false;
}


void stateMachine_run(void)
{
    double time = glfwGetTime();
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
    printf("usage: %.1f % \n", ((glfwGetTime() - time) / frames * 100) / 0.016f);
    window_terminate();
    audio_terminate();
    vector_foreach(&states, State*, state)
    {
        state->destruct(state);
        che_free(state);
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