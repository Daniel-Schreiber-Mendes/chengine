#include "engine.h"


typedef struct
{
	
}
ComponentView;


static GtkTreeStore 	 *trst;
static GtkTreeView 	     *tv;
static GtkTreeViewColumn *name_tvc;
static GtkTreeViewColumn *count_tvc;
static GtkCellRenderer   *name_cr;
static GtkCellRenderer   *count_cr;
static GtkTreeSelection  *trse;
static GtkButton 		 *add_b;
static GtkButton 		 *remove_b;
static GtkTreeModel 	 *model;
static GtkTreeIter 		  iter;
static Vector 			  components;


static void on_add_b_clicked(GtkButton *b, gpointer user_data);
static void on_remove_b_clicked(GtkButton *b, gpointer user_data);
static void on_name_cr_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data);
static void on_count_cr_edited(GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data);
static void load_component(char const *const name, uint16_t const count);


void components_init(GtkBuilder *const builder)
{
	trst = GTK_TREE_STORE(gtk_builder_get_object(builder, "components_trst"));
	tv = GTK_TREE_VIEW(gtk_builder_get_object(builder, "components_tv"));
	name_tvc = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "components_name_tvc"));
	count_tvc = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "components_count_tvc"));
	name_cr = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "components_name_cr"));
	count_cr = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "components_count_cr"));
	trse = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "components_trse"));
	add_b = GTK_BUTTON(gtk_builder_get_object(builder, "components_add_b"));
	remove_b = GTK_BUTTON(gtk_builder_get_object(builder, "components_remove_b"));


	g_signal_connect(add_b, "clicked", G_CALLBACK(on_add_b_clicked), NULL);
	g_signal_connect(remove_b, "clicked", G_CALLBACK(on_remove_b_clicked), NULL);
	g_signal_connect(name_cr, "edited", G_CALLBACK(on_name_cr_edited), NULL);
	g_signal_connect(count_cr, "edited", G_CALLBACK(on_count_cr_edited), NULL);


	gtk_tree_view_column_add_attribute(name_tvc, name_cr, "text", 0);
	gtk_tree_view_column_add_attribute(count_tvc, count_cr, "text", 1);
}


void components_load(void)
{

}


static void on_add_b_clicked(GtkButton *b, gpointer user_data)
{
	load_component("Component", 0);
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


static void load_component(char const *const name, uint16_t const count)
{
	gtk_tree_selection_get_selected(trse, &model, &iter);
	gtk_tree_store_append(trst, &iter, NULL);
	gtk_tree_store_set(trst, &iter, 0, name, -1);
	gtk_tree_store_set(trst, &iter, 1, count, -1);
}