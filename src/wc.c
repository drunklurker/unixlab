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
    size_t n;
    size_t total_longest_line = 0;
    size_t total_byte_count = 0;
    size_t total_line_count = 0;
    size_t total_word_count = 0;

    size_t file_byte_count = 0;
    for (i = optind; i < params_count; i++)
    {
        size_t file_longest_line = 0;
        file_byte_count = 0;
        size_t file_line_count = 0;
        size_t file_word_count = 0;
        FILE* file = fopen(params[i], "r");
        if (file == NULL)
        {
            perror("fopen");
            return -1;
        }
        ssize_t read_bytes;
        while ((read_bytes  = getline(&line, &n, file)) != -1)
        {
            file_byte_count += read_bytes;
            file_line_count++;
            if (read_bytes > file_longest_line)
            {
                file_longest_line = read_bytes;
                if (line[read_bytes-1] == '\n')
                    file_longest_line--;
            }
            if (n <= 1)
                continue;
            file_word_count += count_line_words(line, n, &flags);
        }
        total_byte_count += file_byte_count;
        total_line_count += file_line_count;
        total_word_count += file_word_count;
        if (total_longest_line < file_longest_line)
        {
            total_longest_line = file_longest_line;
        }
        if (flags.l)
            printf("%6d", file_line_count);
        if (flags.w)
            printf("%6d", file_word_count);
        if (flags.c)
            printf("%6d", file_byte_count);
        if (flags.L)
            printf("%6d", file_longest_line);
        printf("  %s\n", params[i]);
    }
    if (total_byte_count != file_byte_count)
    {
        if (flags.l)
            printf("%6d", total_line_count);
        if (flags.w)
            printf("%6d", total_word_count);
        if (flags.c)
            printf("%6d", total_byte_count);
        if (flags.L)
            printf("%6d", total_longest_line);
        printf("  total\n");
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
