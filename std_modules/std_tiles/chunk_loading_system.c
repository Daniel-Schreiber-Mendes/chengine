#include "tiles.h"

#define CHUNK_COUNT 4

static Program chunk_program;
static uint8_t chunk_unload_threshold; //the distance that if exceeded lets the chunk beeing unloaded
static uint8_t chunk_load_threshold;
static void(*chunk_unload_callback)(Chunk const *const);
static EntityId(*chunk_load_callback)(void);
static EntityId chunk_ids[CHUNK_COUNT][CHUNK_COUNT]; //ringbuffer of all entity ids of the chunks that correspond to the array index
static float chunk_size;
static int8_t chunk_offset_x, chunk_offset_y;
static uint8_t chunk_tile_count; //tiles per row
static Texture const *texture;
//should be updated at max once every 4 seconds


static void world_to_chunk_pos(Transform const *const target_t, int8_t *const dest_x, int8_t *const dest_y);


void chunk_loading_system(checs_system_parameters)
{
	checs_component_use(Transform, t);

	checs_component_get(Transform, t, checs_entity_get_by_tag(CameraTag));

	int8_t new_chunk_offset_x, new_chunk_offset_y, delta_chunk_offset_x, delta_chunk_offset_y;
	world_to_chunk_pos(t, &new_chunk_offset_x, &new_chunk_offset_y);
	delta_chunk_offset_x = new_chunk_offset_x - chunk_offset_x;
	delta_chunk_offset_y = new_chunk_offset_y - chunk_offset_y;

	//left and right wrapping
	if (new_chunk_offset_x != chunk_offset_x)
	{
		/* why is CHUNK_COUNT added altough it has no effect, because of modulo CHUNK_COUNT ? Because if new_chunk_offset_x - 1 or new_chunk_offset_x is negative, the array 
			woudl get indexed negatively. since we want to "wrap the index around" if it is negative, if the index is -1, start at the most right index, we just add 
			CHUNK_COUNT which has the same effect*/
		uint8_t column = ((delta_chunk_offset_x == 1 ? -1 : 0) + new_chunk_offset_x + CHUNK_COUNT * 100) % CHUNK_COUNT;
		for (uint8_t i=0; i < CHUNK_COUNT; ++i)
		{
			checs_component_get(Transform, t, chunk_ids[i][column]);
			t->pos[0] += delta_chunk_offset_x * chunk_size * CHUNK_COUNT;
			 //moves left if player moved left because then delta offset is -1 and right if player moved right
		}
	}
	//up and down wraping
	if (new_chunk_offset_y != chunk_offset_y)
	{
		uint8_t row = ((delta_chunk_offset_y == 1 ? -1 : 0) + new_chunk_offset_y + CHUNK_COUNT * 100) % CHUNK_COUNT;
		for (uint8_t i=0; i < CHUNK_COUNT; ++i)
		{
			checs_component_get(Transform, t, chunk_ids[row][i]);
			t->pos[1] += delta_chunk_offset_y * chunk_size * CHUNK_COUNT;
		}
	}

	chunk_offset_x = new_chunk_offset_x;
	chunk_offset_y = new_chunk_offset_y;
}


void chunk_loading_system_init(EntityId(*load_callback)(void), void(*unload_callback)(Chunk const *const), uint8_t const unload_threshold, uint8_t const load_threshold, 
							  uint16_t const tileCount, Texture const *tex, uint16_t const tileSize)
{
	chunk_unload_threshold = unload_threshold;
	chunk_load_threshold = load_threshold;
	chunk_unload_callback = unload_callback;
	chunk_load_callback = load_callback; //this should only LOAD the chunk. the placement and transform is done by the system
	program_construct(&chunk_program, "./resources/shader/instanced_vertex.glsl", "./resources/shader/instanced_fragment.glsl");
	program_uniform1u_set(chunk_program, "u_texture_index", 4);
	program_uniform1u_set(chunk_program, "u_chunk_tile_count", chunk_tile_count = tileCount);
	program_uniform1f_set(chunk_program, "u_tile_size", (float)tileSize / tex->width);
	texture = tex;
	chunk_size = tileCount; //because one currently tile is exactly 1.0f in size


	checs_component_use(Transform, t);
	for (uint16_t i=0; i < CHUNK_COUNT; ++i)
	{
		for (uint16_t j=0; j < CHUNK_COUNT; ++j)
		{
			chunk_ids[i][j] = chunk_load_callback();
			checs_component_get(Transform, t, chunk_ids[i][j]);
			t->pos[0] = chunk_size * j;
			t->pos[1] = chunk_size * i;
		}
	}
}


void chunk_loading_system_terminate(void)
{
	program_destruct(chunk_program);
		for (uint16_t i=0; i < CHUNK_COUNT; ++i)
	{
		for (uint16_t j=0; j < CHUNK_COUNT; ++j)
		{
			checs_entity_erase(chunk_ids[i][j]);
		}
	}
}


static void world_to_chunk_pos(Transform const *const target_t, int8_t *const dest_x, int8_t *const dest_y)
{
	*dest_x = (int)((target_t->pos[0]) / chunk_size) - (target_t->pos[0] < 0);
	*dest_y = (int)((target_t->pos[1]) / chunk_size) - (target_t->pos[1] < 0) + 1;
}



void chunk_construct(EntityId const e)
{
    checs_component_get_once(Renderable, r, e);
    renderable_construct(r);
    r->vertexCount = 6 * chunk_tile_count * chunk_tile_count; //6 vertices per tile
    r->mode = GL_TRIANGLES;
    r->renderableType = ARRAYS;
    r->program = chunk_program;
    r->texture = texture;
}