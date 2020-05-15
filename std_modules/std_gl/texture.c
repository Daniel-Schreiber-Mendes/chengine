#include "gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb/stb_image.h"


void texture_construct_from_file(Texture *const t, char const *const path)
{
	/*opengl expects us to provide the image data in reverse order, but stbi by default loads the data in normal order.
	because of this we have to tell it explicitly to flip the image when loading it*/
	//stbi_set_flip_vertically_on_load(true);
	void *buffer;
	if (!(buffer = stbi_load(path, &t->width, &t->height, &t->channels, 4)))//4 because RGBA
	{
		printf("Failed to load texture file %s\n", path);
		buffer = stbi_load("./resources/error/errorTexture.png", &t->width, &t->height, &t->channels, 4); //4 because RGBA
	}

	glGenTextures(1, &t->id);
	glBindTexture(GL_TEXTURE_2D, t->id);

	//                                                     width of border
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->width, t->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //makes the image sharper instead of interpolating
	
	che_assert(buffer);
	stbi_image_free(buffer);
}


void texture_construct(Texture *const t, uint16_t const width, uint16_t const height)
{
	glGenTextures(1, &t->id);
	glBindTexture(GL_TEXTURE_2D, t->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //makes the image sharper instead of interpolating

	void *buffer = che_malloc(width * height * 4); //4 because 4 bytes per pixel. RGBA 
	//char buffer[width * height * 4]
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (t->width = width), (t->height = height), 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	che_free(buffer);
}


void texture_update_from_buffer(Texture *const t, void *buffer)
{
	//because it upated the whole texture the x and y offset are 0
	//								  |	 |
	che_assert(buffer);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, t->width, t->height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}


void texture_rect_update_from_buffer(Texture *const t, uint16_t const xoffset, uint16_t const yoffset, uint16_t const width, uint16_t const height, void *buffer)
{
	che_assert(buffer);
	glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}


void texture_destruct(Texture const* t)
{
	glDeleteTextures(1, &t->id);
}


void texture_bind(Texture const *const t)
{
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, t->id);
}