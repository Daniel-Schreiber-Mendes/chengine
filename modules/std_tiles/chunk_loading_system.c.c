#include "tiles.h"
/*
#define CHUNK_COUNT 4

static uint8_t chunk_unload_threshold; //the distance that if exceeded lets the chunk beeing unloaded
static uint8_t chunk_load_threshold;
static chunk_load_callback load;
static chunk_unload_callback unload;
static EntityId chunks[CHUNK_COUNT][CHUNK_COUNT]; //ringbuffer of all entity ids of the chunks that correspond to the array index
static int8_t chunk_offset_x, chunk_offset_y;
static uint8_t chunk_tile_count; //tiles per row
//should be updated at max once every 4 seconds


void chunk_loading_task(void)
{

}


void chunk_loading_task_init(uint8_t const unload_threshold, uint8_t const load_threshold, uint16_t const tileCount);
{

}


void chunk_loading_task_terminate(void)
{
	for (uint16_t i=0; i < CHUNK_COUNT; ++i)
	{
		for (uint16_t j=0; j < CHUNK_COUNT; ++j)
		{
			chunk_unload(chunks[i][j]);
		}
	}
}


void chunk_unload(Chunk const *const c)
{
	FILE *file = fopen("file.bin", "w+");
	checs_attribute_entity_foreach(ChunkLoadable, e)
	{
		checs_entity_erase(e);
	}
}*/