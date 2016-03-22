#include "rmdir.h"

#include <unistd.h>

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

    return rmdir(params[optind]);
}
