#include "tilemap.h"


static Chunk chunks[CHUNK_COUNT][CHUNK_COUNT];
static Texture cobble, grass, sand, water;
static Image tileset_image;

void tilemap_import(void)
{
	checs_component_register(9, sizeof(Tile), NULL, NULL, 1 << 1 | 1 << 5);
}


void tilemap_generate(void)
{
	srand(time(NULL));

	tileset_image = textureManager_image_load("./resources/textures/tilemap.png");
    texture_construct(&cobble, 128, 32, 32, 32, &tileset_image);
    texture_construct(&grass, 0, 0, 32, 32, &tileset_image);
    texture_construct(&sand, 64, 32, 32, 32, &tileset_image);
    texture_construct(&water, 160, 0, 32, 32, &tileset_image);
    render_rectBatch_image_set(&tileset_image);

	for (uint8_t cx=0; cx < CHUNK_COUNT; ++cx)
	{
		for (uint8_t cy=0; cy < CHUNK_COUNT; ++cy)
		{
			chunk_generate(cx, cy);
		}
	}
}


void chunk_generate(uint8_t cx, uint8_t cy)
{
	int last_n = 1;
	for (uint8_t tx=0; tx < CHUNK_SIZE; ++tx)
	{
		for (uint8_t ty=0; ty < CHUNK_SIZE; ++ty)
		{
			int n = rand() % 5;
			if (n == 4)
			{
				n = last_n;
			}

			last_n = n;

			EntityId e;
			if (n == 3)
			{
				e = checs_entity(1 << 5 | 1 << 9 | 1 << 1 | 1 << 7);
				Collidable *c = checs_component_get(7, e);
				c->behaviourType = STATIC;
				c->type = AABB;
				c->bb[0] = 0.2;
				c->bb[1] = 0.2;
			}
			else
			{
				e = checs_entity(1 << 5 | 1 << 9 | 1 << 1);
			}

			Renderable *r = checs_component_get(5, e);

			if (n == 0 || n == 1)
				r->texture = &grass;
			else if (n == 2)
				r->texture = &sand;
			else if (n == 3)
				r->texture = &water;

			r->scale[0] = 0.4;
	        r->scale[1] = 0.4;
	        r->type = RT_RECT;

			Transform *t = checs_component_get(1, e);
			t->pos[0] = tx * 0.4 + cx * 6.4;
			t->pos[1] = ty * 0.4 + cy * 6.4;
			t->pos[2] = -0.5;

			chunks[cx][cy].entitys[tx][ty] = e;
		}
	}
}


void chunk_load()
{

}


void chunk_save()
{

}


void tilemap_task(void)
{

}
