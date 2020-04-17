#include "../engine.h"


int main(void)
{
	che_checs_init();
    che_init();

	stateMachine_init();
    stateMachine_run();
    stateMachine_terminate();

    che_terminate();
    checs_terminate();
    return 0;
}