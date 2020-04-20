#include "engine.h"


typedef struct
{
	
}
ComponentView;


enum filetype_t
{
	MODULE,
	FILE
};


static GtkTreeStore 	 		*trst;
static GtkTreeView 	     		*tv;
static GtkTreeSelection  		*trse;
static GtkButton 		 		*create_b;
static GtkButton 		 		*remove_b;
static GtkTreeModel 	 		*model;
static GtkTreeIter 		  		 iter;
static GtkSourceLanguageManager *lm;
static GtkSourceLanguage        *l;
static GtkTextBuffer            *sbuf; //is a GtkSourceBuffer
static GtkTextView    		    *sv; // is a GtkSourceView
//static Vector 			  components;

static void on_save_settings_b_clicked(GtkButton *b, gpointer user_data);
static void load_file(char const *const name, enum filetype_t ft);


void modules_init(GtkBuilder *const builder)
{
	GtkTreeViewColumn *name_tvc = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "modules_name_tvc"));
	GtkCellRenderer   *name_cr = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "modules_name_cr"));
	gtk_tree_view_column_add_attribute(name_tvc, name_cr, "text", 0);


	trst = GTK_TREE_STORE(gtk_builder_get_object(builder, "modules_trst"));
	tv = GTK_TREE_VIEW(gtk_builder_get_object(builder, "modules_tv"));
	trse = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "modules_trse"));
	create_b = GTK_BUTTON(gtk_builder_get_object(builder, "modules_create_b"));
	remove_b = GTK_BUTTON(gtk_builder_get_object(builder, "modules_remove_b"));
	sv = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "modules_sv"));


	g_signal_connect(create_b, "clicked", G_CALLBACK(on_add_b_clicked), NULL);
	g_signal_connect(remove_b, "clicked", G_CALLBACK(on_remove_b_clicked), NULL);
	g_signal_connect(trse, "changed", G_CALLBACK(on_trse_changed), NULL);
}


void modules_load(void)
{
	gtk_tree_selection_get_selected(trse, &model, &iter);
	/* a projpath has to look like this: projects/projName/*/
	char const *const projpath = project_path_get();
	if (!projpath)
	{
		g_print("No project selected");
		return;
	}

	char modules_path[100];
	strcpy(modules_path, projpath);
	strcat(modules_path, "/modules");

	fforeach(modules_path, module, 
	({

		load_file(module->d_name, MODULE);

		char module_path[100];
		strcpy(module_path, modules_path);
		strcat(module_path, "/");
		strcat(module_path, module->d_name);

		fforeach(module_path, file, 
		({
			load_file(file->d_name, FILE);
		}))
	}))
}


static void on_name_cr_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data)
{
	gtk_tree_selection_get_selected(trse, &model, &iter);
	gtk_tree_store_set(trst, &iter, 0, new_text, -1);
}


static void on_count_cr_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data)
{
	gtk_tree_selection_get_selected(trse, &model, &iter);

	char *error_message; //we won't use this
	uint16_t const count = strtoul(new_text, &error_message, 10); //10 means base ten

	gtk_tree_store_set(trst, &iter, 1, count, -1);
}


static void load_file(char const *const name, enum filetype_t ft)
{
	static GtkTreeIter module;
	if (ft == MODULE)
	{
		gtk_tree_store_append(trst, &module &iter);
		gtk_tree_store_set(trst, &module, 0, name, -1);
		return;
	}

	gtk_tree_store_append(trst, &module, &iter);
	gtk_tree_store_set(trst, &module, 0, name, -1);
}