#include "ls.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

int shcmd_ls(char* cmd, char* params[])
{
    int params_count = 0;
    while (params[params_count] != NULL)
        params_count++;
    printf("ls started\n");
    struct ls_struct flags;
    flags.l = flags.a = flags.i = 0;
    char opts[] = "lai";
    int opt;
    while ((opt = getopt(params_count, params, opts)) != -1)
    {
        switch(opt)
        {
        case 'l':
            flags.l = 1;
            break;
        case 'a':
            flags.a = 1;
            break;
        case 'i':
            flags.i = 1;
            break;
        }
    }
    char path[500];
    if (params[optind] == NULL)
        strcpy(path, getenv("PWD"));
    else
        strcpy(path, params[optind]);
    printf("optind = %d\n", optind);
    printf("path = %s\n", path);
    int i;
    for (i = 0; i < params_count; i++)
        printf("params[%d] = %s\n", i, params[i]);
    DIR* dir = opendir(path);
    if (dir == NULL)
        return -1;
    struct dirent* dir_entry;
    while ((dir_entry = readdir(dir)) != NULL)
    {
        printf("%s\n", dir_entry->d_name);
    }

    return closedir(dir);
}
