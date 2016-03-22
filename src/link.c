#include "link.h"

#include <unistd.h>

int shcmd_link(char *cmd, char* params[])
{
    int params_count = 0;
    while (params[params_count] != NULL)
        params_count++;
    char opts[] = "";
    int opt;
    while ((opt = getopt(params_count, params, opts)) != -1)
    {
        switch(opt)
        {
        default:
            break;
        }
    }

    return link(params[optind], params[optind+1]);
}
