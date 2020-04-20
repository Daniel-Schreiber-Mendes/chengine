#include <cheio/io.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    return 0;
}


static void proj_comp(GtkButton *b, gpointer user_data)
{
    char command[50];
    strcpy(command, "cd ");
    strcat(command, gtk_file_chooser_get_filename(open_fcb));
    strcat(command, " && make compile");
    system(command);
    raise(SIGKILL);
}


static void proj_run(GtkButton *b, gpointer user_data)
{
    char command[50];
    strcpy(command, "cd ");
    strcat(command, gtk_file_chooser_get_filename(open_fcb));
    strcat(command, " && make run");
    system(command);
    raise(SIGKILL);
}