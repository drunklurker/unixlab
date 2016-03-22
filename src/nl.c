#include "nl.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define NL_BUFFER_SIZE 1024

int shcmd_nl(char* cmd, char* params[])
{
    int params_count = 0;
    while (params[params_count] != NULL)
    {
        params_count++;
    }

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

    int fd = open(params[optind], O_RDONLY);
    if (fd == -1)
    {
        return -1;
    }
    char buffer[NL_BUFFER_SIZE];

    ssize_t read_bytes;
    int line_number = 0;
    while ((read_bytes = read(fd, buffer, NL_BUFFER_SIZE-1)) > 0)
    {
        buffer[read_bytes] = 0;
        char* line = strtok(buffer, "\n");
        while (line != NULL)
        {
            line_number++;
            printf("%6d  %s\n", line_number, line);
            line = strtok(0, "\n");
        }

    }
    return 0;
}
