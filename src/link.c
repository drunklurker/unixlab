#include "link.h"

#include <unistd.h>

int shcmd_link(char *cmd, char* params[])
{
    int params_count = 0;
    while (params[params_count] != NULL)
        params_count++;
    char opts[] = "s";
    struct link_struct flags;
    flags.s = 0;
    int opt;
    while ((opt = getopt(params_count, params, opts)) != -1)
    {
        switch(opt)
        {
        case 's':
            flags.s = 1;
        default:
            break;
        }
    }
    if (flags.s == 0)
    {
        if (link(params[optind], params[optind+1]) == -1)
        {
            perror("link");
            return -1;
        }
    }
    else
    {
        if(symlink(params[optind], params[optind+1]) == -1)
        {
            perror("symlink");
            return -1;
        }
    }

    return 0;
}
