#include "gl.h"
#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb/stb_image.h"

//this manages one rtexture in which it stores all textures loaded by the user
static RTexture texture;
static uint8_t layer;


//an image is a buffer of colors that can be load from a file
//a texture is a index into an image to indicate where the texture starts and ends
void textureManager_init(void)
{
	rtexture_construct(&texture, GL_TEXTURE_2D_ARRAY);
	rtexture_bind(&texture);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, LAYER_WIDTH, LAYER_HEIGHT, LAYER_COUNT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //makes the image sharper instead of interpolating
}


void textureManager_terminate(void)
{
	rtexture_destruct(&texture);
}


void textureManager_image_load(char const *const path)
{
	rtexture_bind(&texture);
	int width, height, channels;
	void *buffer = stbi_load(path, &width, &height, &channels, 4);
	if (!buffer)//4 because RGBA
	{
		printf("Failed to load texture file %s\n", path);
		buffer = stbi_load("./resources/error/errorTexture.png", &width, &height, &channels, 4); //4 because RGBA
	}

	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer++, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	
	che_assert(buffer);
	stbi_image_free(buffer);
}


uint8_t textureManager_current_layer_get(void)
{
	return layer;
}


void texture_update_from_buffer(Texture *const t, uint16_t const xoffset, uint16_t const yoffset, uint16_t const width, uint16_t const height, void const *const buffer)
{
	che_assert(buffer);
	rtexture_bind(&texture);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, xoffset, yoffset, t->layer, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}


void textureManager_texture_bind(void)
{
	rtexture_bind(&texture);
}
