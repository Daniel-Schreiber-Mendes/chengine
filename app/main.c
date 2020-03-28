#include <gtk/gtk.h>


static void on_main_window_destroy(void);
static void on_execute_button_clicked(void);
static GtkWidget *main_window;
static GtkWidget *on_execute_button;


int main(int argc, char *argv[])
{
    GtkBuilder *builder; 

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "glade/che.glade", NULL);

    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    g_signal_connect(main_window, "destroy", G_CALLBACK(on_main_window_destroy), NULL);
    g_signal_connect(main_window, "destroy", G_CALLBACK(on_execute_button_clicked), NULL);

    g_object_unref(builder);

    gtk_widget_show(main_window);                
    gtk_main();

    return 0;
}


static void on_main_window_destroy(void)
{
    gtk_main_quit();
}


static void on_execute_button_clicked(void)
{
    //system("cd ../core/src/ && rm main.o && make");
}