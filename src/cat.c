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
    struct cat_struct flags;
    flags.E = flags.n = 0;
    char opts[] = "nE";
    int opt;
    while ((opt = getopt(params_count, params, opts)) != -1)
    {
        switch(opt)
        {
        case 'E':
            flags.E = 1;
            break;
        case 'n':
            flags.n = 1;
            break;
            default:
                break;
        }
    }

    int i;
    printf("optind == %d\n", optind);
    for (i = 0; i < params_count; i++)
        printf("%d: %s\n", i, params[i]);
    char buffer[1024];
    int line_number = 1;
    for (i = optind; i < params_count; i++)
    {
        int file_id = open(params[i], O_RDONLY);
        if (file_id == -1)
        {
            perror("open");
            return -1; //error
        }
        if (line_number == 1 && flags.n)
        {
            printf("%6d  ", line_number++);
        }
        ssize_t read_bytes;
        while ((read_bytes = read(file_id, buffer, 1024)) > 0)
        {
            int j;
            for (j = 0; j < read_bytes; j++)
            {
                char symbol = buffer[j];
                if (symbol == '\n')
                {
                    if (flags.E)
                    {
                        printf("$");
                    }
                    printf("\n");
                    if (flags.n)
                    {
                        printf("%6d  ", line_number++);
                    }
                }
                else
                {
                    printf("%c", symbol);
                }
            }
        }
        printf("\n");
        close(file_id);
    }
    return 0;
}
