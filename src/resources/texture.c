#include "../../engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb_image.h"


void texture_construct(Texture *const texture, char const *const path)
{
	/*opengl expects us to provide the image data in reverse order, but stbi by default loads the data in normal order.
	because of this we have to tell it explicitly to flip the image when loading it*/
	stbi_set_flip_vertically_on_load(true);
	if (!(texture->buffer = stbi_load(path, &texture->width, &texture->height, &texture->channels, 4)))//4 because RGBA
	{
		texture->buffer = stbi_load("../resources/error/errorTexture.png", &texture->width, &texture->height, &texture->channels, 4); //4 because RGBA
	}
	
	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //makes the image sharper instead of interpolating
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->buffer);
	
	assert(texture->buffer);
	stbi_image_free(texture->buffer);
}


void texture_destruct(Texture const* texture)
{
	glDeleteTextures(1, &texture->id);
}


void texture_bind(Texture *const texture)
{
	//glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, texture->id);
}