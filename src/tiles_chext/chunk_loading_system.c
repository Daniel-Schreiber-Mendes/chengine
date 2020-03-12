#include "tiles.h"


static uint8_t chunk_unload_threshold; //the distance that if exceeded lets the chunk beeing unloaded
static uint8_t chunk_load_threshold;
static void(*chunk_unload_callback)(Chunk const *const);
static void(*chunk_load_callback)(Chunk *const);
static bool chunk_statuses[3][3];
static EntityId chunk_ids[3][3]; //ringbuffer of all ids of the chunks that correspond to the array index
//should be updated at max once every 4 seconds


static uint8_t manhattan_distance_get(Transform const *const target_t, vec2 const chunk_i);


void chunk_loading_system(checs_system_parameters)
{
	checs_component_use(Chunk, c);
	checs_component_use(Transform, t);
	checs_component_get_once(Transform, camera_t, checs_entity_get_by_tag(CameraTag));


	for (uint16_t i=0; i < 3; ++i)
	{
		for (uint16_t j=0; j < 3; ++j)
		{
			if (manhattan_distance_get(camera_t, (vec2){i, j}) < chunk_load_threshold)
			{
				chunk_load_callback(c);
			}
			else if (manhattan_distance_get(camera_t, (vec2){i, j}) > chunk_unload_threshold)
			{
				chunk_unload_callback(c);
			}
		}
	}
}


void chunk_loading_system_init(void(*load_callback)(Chunk *const), void(*unload_callback)(Chunk const *const), uint8_t const unload_threshold, uint8_t const load_threshold)
{
	chunk_unload_threshold = unload_threshold;
	chunk_load_threshold = load_threshold;
	chunk_unload_callback = unload_callback;
	chunk_load_callback = load_callback;
}


void chunk_loading_system_terminate(void)
{

}


static uint8_t manhattan_distance_get(Transform const *const target_t, vec2 const chunk_i)
{
	//first maps actual world coordinates into chunk coordinates
	//then calculates manhattan distance of the targets and chunks coords
	vec2 target_i = {1, 1};
	return abs(chunk_i[0] - target_i[0]) + abs(chunk_i[1] - target_i[1]);
}



void chunk_construct(EntityId const e, uint16_t const tileSize, uint16_t const textureSize)
{
    checs_component_get_once(Renderable, r, e);
    checs_component_get_once(Transform, t, e);
    transform_construct(t);
    vertexArray_construct(&r->vao);
    r->vertexCount = 6;
    r->primitiveCount = 32 * 32;
    r->mode = GL_TRIANGLES;
    r->renderableType = ARRAYS_INSTANCED;
    program_create(&r->program, "../resources/shader/instanced_vertex.glsl", "../resources/shader/instanced_fragment.glsl");
    program_uniform1f_set(r->program, "u_tile_size", (float)tileSize / textureSize);
    program_uniform1u_set(r->program, "u_texture_index", 4);
    program_uniform1u_set(r->program, "u_chunk_size", 32);
}