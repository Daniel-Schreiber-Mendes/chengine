#include "gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb/stb_image.h"


static GLuint texture_id;


void texture_init(void)
{
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, LAYER_WIDTH, LAYER_HEIGHT, LAYER_COUNT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //makes the image sharper instead of interpolating
}


void texture_terminate(void)
{
	glDeleteTextures(1, &texture_id);
}


void texture_construct_from_file(Texture *const t, char const *const path)
{
	/*opengl expects us to provide the image data in reverse order, but stbi by default loads the data in normal order.
	because of this we have to tell it explicitly to flip the image when loading it*/
	//stbi_set_flip_vertically_on_load(true);
	int channels;
	void *buffer;
	if (!(buffer = stbi_load(path, &t->width, &t->height, &channels, 4)))//4 because RGBA
	{
		printf("Failed to load texture file %s\n", path);
		buffer = stbi_load("./resources/error/errorTexture.png", &t->width, &t->height, &channels, 4); //4 because RGBA
	}

	static uint8_t layer;

	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, t->layer = layer++, t->width, t->height, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	
	che_assert(buffer);
	stbi_image_free(buffer);
}


void texture_rect_update_from_buffer(Texture *const t, uint16_t const xoffset, uint16_t const yoffset, uint16_t const width, uint16_t const height, void *buffer)
{
	che_assert(buffer);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, xoffset, yoffset, t->layer, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}

