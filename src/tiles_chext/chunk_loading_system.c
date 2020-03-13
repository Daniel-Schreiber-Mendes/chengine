#include "tiles.h"

static Program chunk_program;
static uint8_t chunk_unload_threshold; //the distance that if exceeded lets the chunk beeing unloaded
static uint8_t chunk_load_threshold;
static void(*chunk_unload_callback)(Chunk const *const);
static EntityId(*chunk_load_callback)(Chunk *const);
static bool chunk_statuses[5][5] = {false};
static EntityId chunk_ids[5][5] = {0}; //ringbuffer of all entity ids of the chunks that correspond to the array index
static float const chunk_size = 32.0f;
static vec2 chunk_offset = {0, 0};
//should be updated at max once every 4 seconds


static uint8_t manhattan_distance_get(vec2 const target, vec2 const chunk);
static void world_to_chunk_position(Transform const *const target_t, vec2 dest);


void chunk_loading_system(checs_system_parameters)
{
	checs_component_use(Chunk, c);
	checs_component_use(Transform, t);

	checs_component_get(Transform, t, checs_entity_get_by_tag(CameraTag));
	world_to_chunk_position(t, chunk_offset);

	for (uint16_t i=0; i < 5; ++i)
	{
		for (uint16_t j=0; j < 5; ++j)
		{
			if (!chunk_statuses[i][j])
			{
				chunk_statuses[i][j] = true;
				chunk_ids[i][j] = chunk_load_callback(c);
				checs_component_get(Transform, t, chunk_ids[i][j]);
				t->position[0] = chunk_size * i;
				t->position[1] = chunk_size * j;
			}
			if (manhattan_distance_get(chunk_offset, (vec2){i, j}) < chunk_load_threshold)
			{
				//chunk_load_callback(c);
			}
			else if (manhattan_distance_get(chunk_offset, (vec2){i, j}) > chunk_unload_threshold)
			{
				//chunk_unload_callback(c);
			}
		}
	}
}


void chunk_loading_system_init(EntityId(*load_callback)(Chunk *const), void(*unload_callback)(Chunk const *const), uint8_t const unload_threshold, uint8_t const load_threshold)
{
	chunk_unload_threshold = unload_threshold;
	chunk_load_threshold = load_threshold;
	chunk_unload_callback = unload_callback;
	chunk_load_callback = load_callback; //this should only LOAD the chunk. the placement and transform is done by the system
	program_create(&chunk_program, "../resources/shader/instanced_vertex.glsl", "../resources/shader/instanced_fragment.glsl");
}


void chunk_loading_system_terminate(void)
{

}


static uint8_t manhattan_distance_get(vec2 const target, vec2 const chunk)
{
	return abs((chunk[0] + chunk_offset[0]) - target[0]) + abs((chunk[1] + chunk_offset[1])- target[1]);
}


static void world_to_chunk_position(Transform const *const target_t, vec2 dest)
{
	dest[0] = (int)((target_t->position[0]) / chunk_size) - (target_t->position[0] < 0);
	dest[1] = (int)((target_t->position[1]) / chunk_size) - (target_t->position[1] < 0) + 1;
}



void chunk_construct(EntityId const e, uint16_t const tileCount, uint16_t const tileSize, uint16_t const textureSize)
{
    checs_component_get_once(Renderable, r, e);
    checs_component_get_once(Transform, t, e);
    transform_construct(t);
    vertexArray_construct(&r->vao);
    r->vertexCount = 6;
    r->primitiveCount = tileCount * tileCount;
    r->mode = GL_TRIANGLES;
    r->renderableType = ARRAYS_INSTANCED;
    r->program = chunk_program;
    program_uniform1f_set(r->program, "u_tile_size", (float)tileSize / textureSize);
    program_uniform1u_set(r->program, "u_texture_index", 4);
    program_uniform1u_set(r->program, "u_chunk_size", tileCount);
}