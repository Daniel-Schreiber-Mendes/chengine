#ifndef RESOURCES_H
#define RESOURCES_H
#include "../../engine.h"
#include <stdlib.h>
#include <GL/glew.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>


//texture.c
void texture_construct(Texture *const texture, char const *const path);
void texture_destruct(Texture const* texture);
void texture_bind(Texture *const texture);


//file.c
void file_load(File *restrict f, char const *const path);
void file_destruct(File const *const f);

#endif