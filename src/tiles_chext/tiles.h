#ifndef TILES_H
#define TILES_H
#include "../../engine.h"


typedef struct
{
	uint16_t id;
	EntityId tiles[32 * 32];
}
Chunk;


#define CHUNK_LOADING_SYSTEM_COMPONENTS Chunk, Transform

void chunk_construct(EntityId const e, uint16_t const tileSize, uint16_t const textureSize);
void chunk_loading_system_init(void(*callback)(Chunk const *const), float const unload_threshold);
void chunk_loading_system(checs_system_parameters);

#endif