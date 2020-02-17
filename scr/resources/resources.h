#ifndef RESOURCES_H
#define RESOURCES_H

#include <stdlib.h>
#include <GL/glew.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

typedef struct
{
	int width, height, bbp; //bits per pixel
	uint8_t *buffer;
	GLuint id;
}
Texture;


typedef struct
{
	char *buffer;
	uint16_t bufferSize; //size in bytes
}
File;


//texture.c
void texture_construct(Texture *const texture, char const *const path);
void texture_destruct(Texture const* texture);
void texture_bind(Texture *const texture);


//file.c
void file_load(File *restrict f, char const *const path);
void file_destruct(File const *const f);

#endif