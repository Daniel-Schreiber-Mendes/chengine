#include "tiles.h"


static uint8_t chunk_unload_threshold; //the distance that if exceeded lets the chunk beeing unloaded
static uint8_t chunk_load_threshold;
static void(*chunk_unload_callback)(Chunk const *const);
static void(*chunk_load_callback)(Chunk *const);
static bool *chunk_loaded; //represents a two dimensional array
//this unloads a chunk if it is too far away
//should be updated at max once every 4 seconds
void chunk_loading_system(checs_system_parameters)
{
	checs_component_use(Chunk, c);
	checs_component_use(Transform, t);
	checs_component_get_once(Transform, camera_t, checs_entity_get_by_tag(CameraTag));

	checs_entity_foreach(entity)
	{
		checs_component_get(Transform, t, entity);
		checs_component_get(Chunk, c, entity);


		if (transform_distance_get(t, camera_t) < chunk_load_threshold)
		{
			chunk_load_callback(c);
		}
		if (transform_distance_get(t, camera_t) > chunk_unload_threshold)
		{
			chunk_unload_callback(c);
		}
	}
}


void chunk_loading_system_init(void(*load_callback)(Chunk *const), void(*unload_callback)(Chunk const *const), uint8_t const unload_threshold, uint8_t const load_threshold)
{
	chunk_unload_threshold = unload_threshold;
	chunk_load_threshold = load_threshold;
	chunk_unload_callback = unload_callback;
	chunk_load_callback = load_callback;
	chunk_loaded = che_calloc(unload_threshold, unload_threshold);
}


void chunk_loading_system_terminate(void)
{
	che_free(chunk_loaded);
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