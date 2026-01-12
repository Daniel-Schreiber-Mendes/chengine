#include <render/render.h>
#include <stdlib.h>
#include <time.h>       
#include <physics/physics.h>

#define CHUNK_SIZE 16
#define CHUNK_COUNT 12

typedef struct
{
	enum 
	{
		TILE_STATIC,
		TILE_DYNAMIC
	}movement_type;

	enum
	{
		TILE_L0, //background
		TILE_L1	//player
	}layer;

	enum
	{
		TILE_GRASS,
		TILE_SAND,
		TILE_COBBLE
	}type;
}
Tile;


typedef struct
{
	EntityId entitys[CHUNK_SIZE][CHUNK_SIZE]; //static tiles
}
Chunk;

void tilemap_generate(void);
void tilemap_import(void);
void tilemap_task(void);
void physics_partitions_center(vec2 pos);
void chunk_generate(uint8_t cx, uint8_t cy);
