#include <cheio/io.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("No arguments specified\n");
        return 0;
    }

    if (!strcmp(argv[1], "c"))
    {
        if (!strcmp(argv[2], "p"))
        {
            mkdir(".che", 0777);
        }
        else if (!strcmp(argv[2], "m"))
        {
            printf("Created a module\n");
        }
        else
        {
            printf("No type of creation specified\n");
        }
    }
    else if (!strcmp(argv[1], "r"))
    {
        if (!strcmp(argv[2], "p"))
        {
            
        }
        else if (!strcmp(argv[2], "m"))
        {
            if (argc < 4)
            {
                printf("No module name specified\n");
            }
            else
            {
                char command[PATH_MAX] = "rm -r modules/";
                strcat(command, argv[3]);
                system(command);
            }
        }
        else
        {
            printf("No type specified for removal\n");
        }
    }
    else if (!strcmp(argv[1], "i"))
    {
        if (!strcmp(argv[1], "m"))
        {
            
        }
    }
    //getcwd(cwd, sizeof cwd);
    return 0;
}


static void proj_comp(void)
{
    char command[50];
    strcpy(command, "cd ");
    strcat(command, cwd);
    strcat(command, " && make compile");
    system(command);
}


static void mod_comp(void)
{
    system("make compile");
}


static void proj_run(void)
{
    system("make run");
}