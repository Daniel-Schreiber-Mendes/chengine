#ifndef CUSTOM_H
#define CUSTOM_H
#include <chengine-dev/engine.h>
#include "modules/std_tiles/tiles.h"
#include "modules/std_physics/physics.h"
#include "modules/std_render/render.h"
#include "modules/std_camera/camera.h"
#include "modules/std_audio/audio.h"
#include "modules/std_window/window.h"

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

typedef struct
{ 
	int i;	
}
KeyInput;


void input_task(void);


#endif