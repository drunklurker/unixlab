#include "cat.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int shcmd_cat(char* cmd, char* params[])
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

    int i;
    printf("optind == %d\n", optind);
    for (i = 0; i < params_count; i++)
        printf("%d: %s\n", i, params[i]);
    char buffer[1024];
    for (i = optind; i < params_count; i++)
    {
        int file_id = open(params[i], O_RDONLY);
        if (file_id == -1)
            return 1; //error
        ssize_t read_bytes;
        while ((read_bytes = read(file_id, buffer, 1023)) > 0)
        {
            buffer[read_bytes] = 0;
            printf("%s", buffer);
        }
        printf("\n");
        close(file_id);
    }
    return 0;
}
