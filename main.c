#include "engine.h"



int main(int argc, char *argv[])
{
    return 0;
}


static void on_compile_b_clicked(GtkButton *b, gpointer user_data)
{
    char command[50];
    strcpy(command, "cd ");
    strcat(command, gtk_file_chooser_get_filename(open_fcb));
    strcat(command, " && make compile");
    system(command);
    raise(SIGKILL);
}


static void on_run_b_clicked(GtkButton *b, gpointer user_data)
{
    char command[50];
    strcpy(command, "cd ");
    strcat(command, gtk_file_chooser_get_filename(open_fcb));
    strcat(command, " && make run");
    system(command);
    raise(SIGKILL);
}