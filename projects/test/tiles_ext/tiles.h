#ifndef TILES_H
#define TILES_H
#include <chengine-dev/engine.h>
#include "../render_ext/render.h"
#include "../movement_ext/movement.h"


typedef struct
{
	uint16_t id;
}
Chunk;

/*
Requirements:
 - This system has to be initialized AFTER stateMachine_init() and before any state is pushed
 - An entity with Camera component has to have a transform component
*/

#define CHUNK_LOADING_SYSTEM_COMPONENTS Chunk, Transform

void chunk_construct(EntityId const e);
void chunk_loading_system_init(EntityId(*load_callback)(void), void(*unload_callback)(Chunk const *const), uint8_t const unload_threshold, uint8_t const load_threshold,
							   uint16_t const tileCount, Texture const *tex, uint16_t const tileSize);
void chunk_loading_system_terminate(void);
void chunk_loading_system_tileData_set(uint16_t const tileCount, float const relativeTileSize);
void chunk_loading_system(checs_system_parameters);

#endif