#include "rmdir.h"

#include <unistd.h>
#include <stdio.h>

int shcmd_rmdir(char* cmd, char* params[])
{
    int opt;
    char opts[] = "";
    int param_count = 0;
    while (params[param_count] != NULL)
        param_count++;
    while ((opt=getopt(param_count, params, opts)) != -1)
    {
        switch (opt)
        {
        default:
            break;
        }
    }

    if (rmdir(params[optind])!= 0)
    {
        perror("rmdir error\n");
        return -1;
    }

    return 0;
}
