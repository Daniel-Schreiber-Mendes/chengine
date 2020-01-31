#ifndef RESOURCES
#define RESOURCES

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

typedef struct
{
	int width, height, bbp; //bits per pixel
	uint8_t *buffer;
	GLuint id;
}Texture;


typedef struct
{
	char* buffer;
	uint64_t bufferSize; //size in bytes
}File;


//texture.c
void	texture_construct(Texture *const texture, char const *const path);
void	texture_destruct(Texture const* texture);
void	texture_bind(Texture *const texture);


//file.c
void file_load(File *const f, char const *const path);
void file_destruct(File *const f);
#endif