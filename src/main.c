#include "../engine.h"


int main(void)
{
	checs_init_config();

	audio_init();
    window_init();

    che_window_config();
    che_init_config();

	stateMachine_init();
	stateMachine_first_state_push();
    stateMachine_run();
    stateMachine_terminate();

    che_terminate_config();
    
    checs_terminate();
    window_terminate();
    audio_terminate();

    return 0;
}