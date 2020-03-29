#ifndef IO_H
#define IO_H
#include "debug_utils.h"
#include <stdbool.h>
#include <stdint.h>


typedef struct
{
	void *buffer;
	uint16_t bufferSize; //size in bytes
}
File;


typedef struct
{
	void *buffer; //this can be casted to a File
	uint16_t bufferSize; //size in bytes
	char *keys;
	char *values;
}
ConfigFile;


//io.c

bool file_construct(File *const f, char const *const path);
bool file_construct_bin(File *const f, char const *const path);
void file_destruct(File const *const f);


#endif