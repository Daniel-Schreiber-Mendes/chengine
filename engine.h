#ifndef CHENGINE_H
#define CHENGINE_H
#include <cheio/io.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>


void settings_init(GtkBuilder *const builder);
void modules_init(GtkBuilder *const builder);
void modules_load(void);
char* project_path_get(void);



#endif