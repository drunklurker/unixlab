#include "rm.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

int shcmd_rm(char* cmd, char* params[])
{
    int params_count = 0;
    while (params[params_count] != NULL)
        params_count++;
    printf("rm started\n");
    struct stat info;
    struct rm_struct flags;
    flags.r = flags.f = 0;
    char opts[] = "rf";
    int opt;
    while ((opt = getopt(params_count, params, opts)) != -1)
    {
        switch(opt)
        {
        case 'r':
            flags.r = 1;
            break;
        case 'f':
            flags.f = 1;
            break;
        }
    }
    int i;
    printf("optind == %d\n", optind);
    for (i = 0; i < params_count; i++)
        printf("%d: %s\n", i, params[i]);
    lstat(params[1], &info);
    int isDir = S_ISDIR(info.st_mode);
    printf("this is a %s\n", isDir > 0 ? "folder" : "file");
    if (!isDir)
    {
        //target object is a file
        remove(params[optind]);
    }
    else
    {
        //target object is a directory
        if (!flags.r)
        {
            char errorString[1024];
            sprintf(errorString, "rm: cannot remove '%s': Is a directory\n", params[optind]);
            perror(errorString);
            return -1;
        }
        else
        {
            remove(params[optind]);
        }
    }
    return 0;
}
