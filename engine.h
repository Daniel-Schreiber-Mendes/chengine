#ifndef CHENGINE_H
#define CHENGINE_H
#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>
#include <checl/containers.h>
#include <cheio/io.h>


void settings_init(GtkBuilder *const builder);
void components_init(GtkBuilder *const builder);
void components_load(void);
char* project_path_get(void);


#endif