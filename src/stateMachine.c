#include "../engine.h"
static Vector states; 
static bool running = true;


void stateMachine_init(void)
{	 
    vector_construct(&states, sizeof(State*));
}


//a state shall only be popped inside state.update, NEVER in state.draw
void stateMachine_state_pop(State *const state)
{
    vector_erase(&states, State*, vector_find(&states, state));
    state->destruct(state);
    che_free(state);
}


void stateMachine_run(void)
{
    stateMachine_first_state_push();
	while(running)
	{                
        glfwPollEvents(); 
        checs_eventBuffers_swap();
        vector_foreach(&states, State*, state)
        {      
            state->draw(state);
            state->update(state); 
            //it is important that the draw function is called before update because when a state is popped in update and then the draw
            //function would be called, one would access memory that already got freed
        }
	}
}


void stateMachine_terminate(void)
{
    vector_foreach(&states, State*, state)
    {
        state->destruct(state);
        che_free(state);
    }
    vector_destruct(&states);
}


void _stateMachine_state_push(State *const state)
{
    vector_push_back(&states, State*, state);
}


void stateMachine_running_set(bool const n_running)
{
    running = n_running;
}