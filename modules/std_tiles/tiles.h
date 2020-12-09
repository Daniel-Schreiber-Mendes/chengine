#ifndef TILES_H
#define TILES_H
#include <chengine-dev/engine.h>
#include "../std_render/render.h"
#include "../std_movement/movement.h"
#include "stdio.h"



//checs_attribute_define(ChunkLoadable);
typedef struct {} ChunkLoadable;
/*
typedef uint16_t ChunkI;


typedef struct
{
	int8_t x, y;
}
Chunk;

//attribute ChunkLoadable


typedef void(*chunk_load_callback)(ChunkId, Chunk*);
typedef void(*chunk_unload_callback)(Chunk const*);


void chunk_loading_task_init(uint8_t unload_threshold, uint8_t load_threshold, uint16_t tileCount);
void chunk_loading_task_terminate(void);
void chunk_loading_task(void);
void chunk_unload(Chunk const *c);
*/
#endif