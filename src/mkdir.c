#include "mkdir.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

int shcmd_mkdir(char* cmd, char* params[])
{
    printf("mkdir started\n");
    char opts[] = "p";
    struct mkdir_struct flags;
    flags.p = 0;
    int params_count = 0;
    while (params[params_count] != NULL)
        params_count++;
    flags.p = 0;
    int opt;
    while ((opt = getopt(params_count, params, opts)) != -1)
    {
        switch(opt)
        {
        case 'p':
            flags.p = 1;
            break;
        }
    }
    if (mkdir(params[optind], 0777) == -1)
    {
        perror("Could not create directory");
    }
    return 0;
}
