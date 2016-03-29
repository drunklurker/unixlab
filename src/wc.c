#include "wc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int shcmd_wc(char* cmd, char* params[])
{
    int params_count = 0;
    while (params[params_count] != NULL)
        params_count++;
    struct wc_struct flags;
    flags.c = flags.l = flags.w = flags.L = 0;
    char opts[] = "cwlL";
    int opt;
    while ((opt = getopt(params_count, params, opts)) != -1)
    {
        switch(opt)
        {
        case 'c':
            flags.c = 1;
            break;
        case 'w':
            flags.w = 1;
            break;
        case 'l':
            flags.l = 1;
            break;
        case 'L':
            flags.L = 1;
            break;
        }
    }
    int i;
    printf("optind == %d\n", optind);
    for (i = 0; i < params_count; i++)
        printf("%d: %s\n", i, params[i]);
    if (flags.c == flags.w && flags.w == flags.l && flags.l == flags.L && flags.L == 0)
    {
        flags.c = flags.w = flags.l = 1;
    }
    char* line = NULL;
    for (i = optind; i < params_count; i++)
    {
        size_t longest_line = 0;
        size_t file_byte_count = 0;
        size_t file_line_count = 0;
        size_t file_word_count = 0;
        size_t n;
        FILE* file = fopen(params[i], "r");
        if (file == NULL)
        {
            perror("fopen");
            return -1;
        }
        ssize_t read_bytes;
        while ((read_bytes  = getline(&line, &n, file)) != -1)
        {
            file_byte_count += n;
            file_line_count++;
            if (n > longest_line)
                longest_line = n;
            if (n <= 1)
                continue;
            file_word_count += count_line_words(line, n, &flags);
        }
        if (flags.l)
            printf("  %d", file_line_count);
        if (flags.w)
            printf("  %d", file_word_count);
        if (flags.c)
            printf("  %d", file_byte_count);
        if (flags.L)
            printf("  %d", longest_line);
        printf("  %s\n", params[i]);
    }
    if (line != NULL)
        free(line);
    return 0;
}

int count_line_words(char* line, size_t length, struct wc_struct* flags)
{
    char delim[] = " \t\n";
    char* word = strtok(line, delim);
    int word_count = 0;
    if (word == NULL)
        return word_count;
    word_count++;
    while ((word = strtok(NULL, delim)) != NULL)
        word_count++;
    return word_count;
}
