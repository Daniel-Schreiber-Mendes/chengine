#ifndef CUSTOM_H
#define CUSTOM_H
#include <chengine-dev/engine.h>
#include "./tiles_ext/tiles.h"
#include "./physics_ext/physics.h"
#include "./render_ext/render.h"
#include "./camera_ext/camera.h"

//components defined by extensions:
//+1 for Collidable and +1 for Chunk
#define ComponentCount StandardComponentCount + 2

typedef struct
{
	State b;
}
MainMenuState;


void mainMenuState_construct(MainMenuState *const s);
void mainMenuState_destruct(State *const state);
void mainMenuState_update(State *const state);
void mainMenuState_draw(State *const state);

/* Requirements:
 - If an entity has a KeyInput component, it has to have a Velocity component 
*/
void input_task(void);


#endif