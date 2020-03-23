#ifndef CUSTOM_H
#define CUSTOM_H
#include "../../engine.h"
#include "../tiles_chext/tiles.h"
#include "../physics_chext/physics.h"

//components defined by extensions:
//+1 for Collidable and +1 for Chunk
#define ComponentCount StandardComponentCount + 2

typedef struct
{
	State b;
}
MainMenuState;

extern Texture cobble;


void mainMenuState_construct(MainMenuState *const s);
void mainMenuState_destruct(State *const state);
void mainMenuState_update(State *const state);
void mainMenuState_draw(State *const state);

/* Requirements:
 - If an entity has a KeyInput component, it has to have a Velocity component 
*/
void input_task(void);

/*Requirements:
 - If an entity has a Velocity component it has to have a transform component
*/
void movement_task(void);


#endif