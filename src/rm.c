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
    //count the parameters
    int params_count = 0;
    while (params[params_count] != NULL)
        params_count++;
    printf("rm started\n");
    struct stat info;
    //process options
    struct rm_struct flags;
    flags.r = 0;
    char opts[] = "r";
    int opt;
    while ((opt = getopt(params_count, params, opts)) != -1)
    {
        switch(opt)
        {
        case 'r':
            flags.r = 1;
            break;
        }
    }
    int i;
    //debug output
    printf("optind == %d\n", optind);
    for (i = 0; i < params_count; i++)
        printf("%d: %s\n", i, params[i]);
    //get information about target file or directory
    lstat(params[1], &info);
    //check if it's a file or directory - relevant for -r flag
    int isDir = S_ISDIR(info.st_mode);
    //moar debug output
    printf("this is a %s\n", isDir > 0 ? "folder" : "file");
    if (!isDir)
    {
        //target object is a file - remove without thinking
        remove(params[optind]);
    }
    else
    {
        //target object is a directory - check if -r flag is present
        //output error if it's not
        if (!flags.r)
        {
            char errorString[1024];
            sprintf(errorString, "rm: cannot remove '%s': Is a directory\n", params[optind]);
            perror(errorString);
            return -1;
        }
        else //
        {
            remove(params[optind]);
        }
    }
    return 0;
}
