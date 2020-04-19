#include "engine.h"


typedef struct
{
	
}
ComponentView;


static GtkTreeStore 	 		*trst;
static GtkTreeView 	     		*tv;
static GtkTreeSelection  		*trse;
static GtkButton 		 		*add_b;
static GtkButton 		 		*remove_b;
static GtkTreeModel 	 		*model;
static GtkTreeIter 		  		 iter;
static GtkSourceLanguageManager *lm;
static GtkSourceLanguage        *l;
static GtkTextBuffer            *sbuf; //is a GtkSourceBuffer
static GtkTextView    		    *sv; // is a GtkSourceView
//static Vector 			  components;


static void on_add_b_clicked(GtkButton *b, gpointer user_data);
static void on_remove_b_clicked(GtkButton *b, gpointer user_data);
static void on_name_cr_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data);
static void on_count_cr_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data);
static void load_module(char const *const name);
static void on_trse_changed(GtkTreeSelection *treeselection, gpointer user_data);


void modules_init(GtkBuilder *const builder)
{
	GtkTreeViewColumn *name_tvc = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "modules_name_tvc"));
	GtkCellRenderer   *name_cr = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "modules_name_cr"));
	gtk_tree_view_column_add_attribute(name_tvc, name_cr, "text", 0);


	trst = GTK_TREE_STORE(gtk_builder_get_object(builder, "modules_trst"));
	tv = GTK_TREE_VIEW(gtk_builder_get_object(builder, "modules_tv"));
	trse = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "modules_trse"));
	add_b = GTK_BUTTON(gtk_builder_get_object(builder, "modules_add_b"));
	remove_b = GTK_BUTTON(gtk_builder_get_object(builder, "modules_remove_b"));
	sv = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "modules_sv"));


	g_signal_connect(add_b, "clicked", G_CALLBACK(on_add_b_clicked), NULL);
	g_signal_connect(remove_b, "clicked", G_CALLBACK(on_remove_b_clicked), NULL);
	g_signal_connect(trse, "changed", G_CALLBACK(on_trse_changed), NULL);



	lm = gtk_source_language_manager_new();
	l = gtk_source_language_manager_guess_language(lm, "file.c", NULL);

	sbuf = GTK_TEXT_BUFFER(gtk_source_buffer_new(NULL));
	gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(sbuf), l);
	gtk_text_view_set_buffer(sv, sbuf);
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
		char module_path[100];
		strcpy(module_path, modules_path);
		strcat(module_path, "/");
		strcat(module_path, module->d_name);
		fforeach(module_path, file, 
		({
			load_module(file->d_name);
		}))
	}))
}


static void on_add_b_clicked(GtkButton *b, gpointer user_data)
{
	//load_module("Component", 0);
}


static void on_remove_b_clicked(GtkButton *b, gpointer user_data)
{
	if (gtk_tree_selection_get_selected(trse, &model, &iter) == FALSE)
	{
		return;
	}

	char const *name;
	gtk_tree_model_get(model, &iter, 0, &name, -1);
	gtk_tree_store_remove(trst, &iter); //removes the currently selected row from the tree store
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


static void load_module(char const *const name)
{
	gtk_tree_store_append(trst, &iter, NULL);
	gtk_tree_store_set(trst, &iter, 0, name, -1);
}


static void on_trse_changed(GtkTreeSelection *treeselection, gpointer user_data)
{
	g_print("Selection Changed\n");
}