#include "../../engine.h"


//path has to be relative to the executable
void file_load_text(File *const f, char const *const path)
{
    FILE* file = fopen(path, "r");

    assert(file);

    fseek(file, 0, SEEK_END); //move file pointer to end of file
    f->bufferSize = ftell(file); //tell position of pointer which means length of string
    fseek(file, 0, SEEK_SET); //move file pointer back to file begin

    f->buffer = malloc(f->bufferSize);
    fread(f->buffer, f->bufferSize, 1, file); //copy memory in file to buffer
    ((char*)f->buffer)[f->bufferSize] = '\0'; //the string has to end with a null terminator, so put it at the end of it

    fclose(file);
}


void file_load_binary(File *const f, char const *const path)
{
    FILE* file = fopen(path, "rb");

    assert(file);

    fseek(file, 0, SEEK_END); //move file pointer to end of file
    f->bufferSize = ftell(file); //tell position of pointer which means length of string
    fseek(file, 0, SEEK_SET); //move file pointer back to file begin

    f->buffer = malloc(f->bufferSize);
    fread(f->buffer, f->bufferSize, 1, file); //copy memory in file to buffer

    fclose(file);
}


void file_destruct(File const *const f)
{
	free(f->buffer);
}