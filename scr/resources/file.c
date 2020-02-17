#include "resources.h"


void file_load(File *const f, char const *const path)
{
    FILE* file = fopen(path, "r");

    assert(file);

    fseek(file, 0, SEEK_END); //move file pointer to end of file
    f->bufferSize = ftell(file); //tell position of pointer which means length of string
    fseek(file, 0, SEEK_SET); //move file pointer back to file begin

    f->buffer = (char*)malloc(f->bufferSize);
    fread(f->buffer, sizeof(char), f->bufferSize, file); //copy memory in file to buffer
    f->buffer[f->bufferSize] = '\0'; //the string has to end with a null terminator, so put it at the end of it

    fclose(file);
}


void file_destruct(File const *const f)
{
	free(f->buffer);
}