#include "engine.h"


static GtkButton   *save_settings_b;
static GtkComboBox *version_cb;
static GtkComboBox *mode_cb;
static GtkComboBox *optimizations_cb;
static GtkEntry    *librarys_e;


static void on_save_settings_b_clicked(GtkButton *b, gpointer user_data);


void settings_init(GtkBuilder *const builder)
{
	save_settings_b =  GTK_BUTTON(gtk_builder_get_object(builder, "save_settings_b"));
	version_cb =       GTK_COMBO_BOX(gtk_builder_get_object(builder, "version_cb"));
	mode_cb =          GTK_COMBO_BOX(gtk_builder_get_object(builder, "mode_cb"));
	optimizations_cb = GTK_COMBO_BOX(gtk_builder_get_object(builder, "optimizations_cb"));
	librarys_e = 	   GTK_ENTRY(gtk_builder_get_object(builder, "librarys_e"));
	g_signal_connect(save_settings_b, "clicked", G_CALLBACK(on_save_settings_b_clicked), NULL);
}


static void on_save_settings_b_clicked(GtkButton *b, gpointer user_data)
{
	char path[50];

	if (!project_path_get())
	{
		g_print("Failed to save settings because the project path is not set\n");
		return;
	}
	strcpy(path, project_path_get());
	strcat(path, "/Makefile");

	File old_file;
	if (!file_construct(&old_file, path, "r"))
	{
		g_print("Failed to save settings because the Makefile couldnt be found\n");
		file_destruct(&old_file);
		return;
	}
	if (!old_file.bufferSize)
	{
		g_print("Makefile is empty");
		file_destruct(&old_file);
		return;
	}
	remove(path); //delete original Makefile	

	
	FILE *new_file = fopen(path, "w");
	fprintf(new_file, "VERSION = -std=%s\n", gtk_combo_box_get_active_id(version_cb));
	fprintf(new_file, "LIBS = %s\n", gtk_entry_get_text(librarys_e));
	fprintf(new_file, "MODE = %s\n", gtk_combo_box_get_active_id(mode_cb)[0] == 'd' ? "-g" : "");
	fprintf(new_file, "OPTIMIZATIONS = -%s\n", gtk_combo_box_get_active_id(optimizations_cb));
	fputs(strfndch(old_file.buffer, '\n', 4), new_file);
	file_destruct(&old_file);	
	fclose(new_file);
}