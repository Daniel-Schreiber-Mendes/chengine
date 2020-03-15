#include "tiles.h"

#define CHUNK_COUNT 5

static Program chunk_program;
static uint8_t chunk_unload_threshold; //the distance that if exceeded lets the chunk beeing unloaded
static uint8_t chunk_load_threshold;
static void(*chunk_unload_callback)(Chunk const *const);
static EntityId(*chunk_load_callback)(void);
static EntityId chunk_ids[CHUNK_COUNT][CHUNK_COUNT]; //ringbuffer of all entity ids of the chunks that correspond to the array index
static float chunk_size;
static int8_t chunk_offset_x, chunk_offset_y;
static uint8_t chunk_tile_count; //tiles per row
//should be updated at max once every 4 seconds


static uint8_t manhattan_distance_get(vec2 const target, vec2 const chunk);
static void world_to_chunk_position(Transform const *const target_t, int8_t *const dest_x, int8_t *const dest_y);


void chunk_loading_system(checs_system_parameters)
{
	checs_component_use(Transform, t);

	checs_component_get(Transform, t, checs_entity_get_by_tag(CameraTag));

	int8_t new_chunk_offset_x, new_chunk_offset_y;
	world_to_chunk_position(t, &new_chunk_offset_x, &new_chunk_offset_y);

	if (chunk_offset_x - new_chunk_offset_x == -1)
	{
		chunk_offset_x += new_chunk_offset_x - chunk_offset_x;
		for (uint16_t i=0; i < CHUNK_COUNT; ++i)
		{
			checs_component_get(Transform, t, chunk_ids[i][((chunk_offset_x - 1) % CHUNK_COUNT)]);
			t->position[0] += chunk_size * CHUNK_COUNT;
		}
	}
	else if (chunk_offset_x - new_chunk_offset_x == 1)
	{
		chunk_offset_x += new_chunk_offset_x - chunk_offset_x;
		for (uint16_t i=0; i < CHUNK_COUNT; ++i)
		{
			checs_component_get(Transform, t, chunk_ids[i][((chunk_offset_x + 1) % CHUNK_COUNT)]);
			t->position[0] -= chunk_size * CHUNK_COUNT;
		}
	}

	if (chunk_offset_y - new_chunk_offset_y)
	{
		chunk_offset_y += new_chunk_offset_y - chunk_offset_y;
		for (uint16_t i=0; i < CHUNK_COUNT; ++i)
		{
			checs_component_get(Transform, t, chunk_ids[((chunk_offset_y - 1) % CHUNK_COUNT)][i]);
			t->position[1] += chunk_size * CHUNK_COUNT;
		}
	}


	chunk_offset_x = new_chunk_offset_x;
	chunk_offset_y = new_chunk_offset_y;
}


void chunk_loading_system_init(EntityId(*load_callback)(void), void(*unload_callback)(Chunk const *const), uint8_t const unload_threshold, uint8_t const load_threshold, 
							  uint16_t const tileCount, float const relativeTileSize)
{
	chunk_unload_threshold = unload_threshold;
	chunk_load_threshold = load_threshold;
	chunk_unload_callback = unload_callback;
	chunk_load_callback = load_callback; //this should only LOAD the chunk. the placement and transform is done by the system
	program_create(&chunk_program, "../resources/shader/instanced_vertex.glsl", "../resources/shader/instanced_fragment.glsl");
	program_uniform1u_set(chunk_program, "u_texture_index", 4);
	program_uniform1u_set(chunk_program, "u_chunk_tile_count", chunk_tile_count = tileCount);
	program_uniform1f_set(chunk_program, "u_tile_size", relativeTileSize);
	chunk_size = tileCount; //because one currently tile is exactly 1.0f in size


	checs_component_use(Transform, t);
	for (uint16_t i=0; i < CHUNK_COUNT; ++i)
	{
		for (uint16_t j=0; j < CHUNK_COUNT; ++j)
		{
			chunk_ids[i][j] = chunk_load_callback();
			checs_component_get(Transform, t, chunk_ids[i][j]);
			t->position[0] = chunk_size * j;
			t->position[1] = chunk_size * i;
		}
	}
}


static uint8_t manhattan_distance_get(vec2 const target, vec2 const chunk)
{
	return abs((chunk[0] + chunk_offset_x) - target[0]) + abs((chunk[1] + chunk_offset_y)- target[1]);
}


static void world_to_chunk_position(Transform const *const target_t, int8_t *const dest_x, int8_t *const dest_y)
{
	*dest_x = (int)((target_t->position[0]) / chunk_size) - (target_t->position[0] < 0);
	*dest_y = (int)((target_t->position[1]) / chunk_size) - (target_t->position[1] < 0) + 1;
}



void chunk_construct(EntityId const e, float const relativeTileSize)
{
    checs_component_get_once(Renderable, r, e);
    renderable_construct(r);
    r->vertexCount = 6 * chunk_tile_count * chunk_tile_count; //6 vertices per tile
    r->mode = GL_TRIANGLES;
    r->renderableType = ARRAYS;
    r->program = chunk_program;
}