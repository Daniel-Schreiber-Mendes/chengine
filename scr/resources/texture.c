#include "resources.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb_image.h"


void texture_construct(Texture *const texture, char const *const path)
{
	texture->buffer = stbi_load(path, &texture->width, &texture->height, &texture->bbp, 4); //4 because RGBA

	glGenTextures(1, &texture->id);
	texture_bind(texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->buffer);

	if (texture->buffer)
		stbi_image_free(texture->buffer);
	else
		assert(false);
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