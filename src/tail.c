#include "tail.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int shcmd_tail(char* cmd, char* params[])
{
    //count parameters
    int params_count = 0;
    while (params[params_count] != NULL)
        params_count++;
    //struct for flags
    struct tail_struct flags;
    //default number of lines to output
    flags.n = 10;
    //a flag for -n +k case
    flags.last = 1;
    char opts[] = "n:";
    int opt;
    while ((opt = getopt(params_count, params, opts)) != -1)
    {
        switch(opt)
        {
        case 'n':
            //if optarg starts with +, output lines starting with optarg
            if (optarg[0] == '+')
            {
                flags.last = 0;
                flags.n = atoi(optarg+1);
            }
            else //else output last optarg lines
            {
                flags.last = 1;
                flags.n = atoi(optarg);
            }
            break;
        }
    }
    int i;

    //n cannot be negative
    if (flags.n <= 0)
    {
        perror("I see what you did there\n");
        return -1;
    }

    //count files to decide whether to print their paths or not
    int file_count = 0;
    i = optind;
    while (params[i] != NULL)
    {
        file_count++;
        i++;
    }

    //read each file
    for (i = optind; i < params_count; i++)
    {
        //open file as stream to use getline function
        FILE* file = fopen(params[i], "r");
        if (file == NULL)
        {
            perror("fopen");
            return -1;
        }
        //if we need last N lines
        if (flags.last)
        {
            if (file_count > 1)
                print_last_lines(file, flags.n, params[i]);
            else
                print_last_lines(file, flags.n, NULL);
        }
        else //or if we need all lines starting with N
        {
            if (file_count > 1)
                print_lines_starting_with(file, flags.n, params[i]);
            else
                print_lines_starting_with(file, flags.n, NULL);
        }
        fclose(file);
        printf("\n");
    }
    return 0;
}

int print_last_lines(FILE* file, int line_count, char* path)
{
    ssize_t read_bytes;
    char* line = NULL;
    size_t n = 0;
    int i;
    //allocate memory for our string "window"
    char** strings = malloc(line_count*sizeof(strings[0]));
    int lines_read = 0;

    while ((read_bytes  = getline(&line, &n, file)) != -1)
    {
        //while "window" isn't full, just add string to it
        if (lines_read < line_count)
        {
            strings[lines_read++] = line;
        }
        else //if it is full, delete, the oldest string and shift the others, then add the new one
        {
            free(strings[0]);
            for (i = 1; i < line_count; i++)
                strings[i-1] = strings[i];
            strings[line_count-1] = line;
        }
        line = NULL; //assign NULL to our buffer pointer so that it does not overwrite existing strings
        n = 0;
    }
    //check if we need to print filename
    if (path != NULL)
    {
        printf("==> %s <==\n", path);
    }
    //output all strings in the window and free the memory
    for (i = 0; i < lines_read; i++)
    {
        printf("%s", strings[i]);
        free(strings[i]);
    }

    free(strings);
    return 0;
}

int print_lines_starting_with(FILE* file, int lines_to_skip, char* path)
{
    ssize_t read_bytes;
    char* line = NULL;
    size_t n = 0;
    int lines_read = 1;
    if (path != NULL)
    {
        printf("==> %s <==\n", path);
    }

    while ((read_bytes  = getline(&line, &n, file)) != -1)
    {
        //if we haven't reached the Nth string, just increase counter
        if (lines_read < lines_to_skip)
            lines_read++;
        else //otherwise print the string
            printf("%s", line);
    }
    free(line);

    return 0;
}
