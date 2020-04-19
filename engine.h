#ifndef CHENGINE_H
#define CHENGINE_H
#include <gtksourceview/gtksource.h>
#include <gtksourceview/gtksourcelanguagemanager.h>
#include <gtk/gtk.h>
#include <checl/containers.h>
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