#include "engine.h"

static void on_main_window_destroy(GtkWidget *widget, GdkEvent *event, gpointer user_data);
static void on_compile_b_clicked(GtkButton *b, gpointer user_data);
static void on_run_b_clicked(GtkButton *b, gpointer user_data);
static void on_open_fcb_file_set(GtkFileChooserButton *widget, gpointer user_data);


static GtkWindow      *main_window;
static GtkButton      *compile_b;
static GtkButton      *run_b;
static GtkFileChooser *open_fcb;


int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    g_type_ensure(GTK_SOURCE_TYPE_VIEW);
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "che.glade", NULL);

    
    main_window = GTK_WINDOW(gtk_builder_get_object(builder, "main_w"));
    
    compile_b = GTK_BUTTON(gtk_builder_get_object(builder, "compile_b"));
    run_b = GTK_BUTTON(gtk_builder_get_object(builder, "run_b"));
    open_fcb = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "open_fcb"));
    gtk_file_chooser_set_action(GTK_FILE_CHOOSER(open_fcb), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
    
    settings_init(builder);
    modules_init(builder);
    
    g_signal_connect(main_window, "destroy", G_CALLBACK(on_main_window_destroy), NULL);
    g_signal_connect(compile_b, "clicked", G_CALLBACK(on_compile_b_clicked), NULL);
    g_signal_connect(run_b, "clicked", G_CALLBACK(on_run_b_clicked), NULL);
    g_signal_connect(open_fcb, "file-set", G_CALLBACK(on_open_fcb_file_set), NULL);


    g_object_unref(builder);

    gtk_widget_show(GTK_WIDGET(main_window));       
    gtk_main();

    return 0;
}


static void on_main_window_destroy(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    gtk_main_quit();
}


static void on_compile_b_clicked(GtkButton *b, gpointer user_data)
{
    if (fork() == 0) //create child process to compile the application
    {
        char command[50];
        strcpy(command, "cd ");
        strcat(command, gtk_file_chooser_get_filename(open_fcb));
        strcat(command, " && make compile");
        system(command);
        raise(SIGKILL);
    }
}


static void on_run_b_clicked(GtkButton *b, gpointer user_data)
{
    if (fork() == 0) //create child process to run the application
    {
        char command[50];
        strcpy(command, "cd ");
        strcat(command, gtk_file_chooser_get_filename(open_fcb));
        strcat(command, " && make run");
        system(command);
        raise(SIGKILL);
    }
}


char* project_path_get(void)
{
    return gtk_file_chooser_get_filename(open_fcb);
}


static void on_open_fcb_file_set(GtkFileChooserButton *widget, gpointer user_data)
{
    modules_load();
}