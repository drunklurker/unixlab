#include "grep.h"

#include <unistd.h>
#include <regex.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int shcmd_grep(char *cmd, char* params[])
{
int params_count = 0;
    while (params[params_count] != NULL)
        params_count++;
    char opts[] = "c";
    int i;
    struct grep_struct flags;
    flags.c = 0;
    int opt;
    while ((opt = getopt(params_count, params, opts)) != -1)
    {
        switch(opt)
        {
        case 'c':
            flags.c = 1;
        default:
            break;
        }
    }
    printf("optind == %d\n", optind);
    for (i = 0; i < params_count; i++)
        printf("%d: %s\n", i, params[i]);
    char* pattern_string = params[optind];
    regex_t pattern_regex;
    if (regcomp(&pattern_regex, pattern_string, REG_NOSUB) != 0)
    {
        perror("regcomp\n");
        return -1;
    }

    int file_count = 0;
    i = optind+1;
    while (params[i] != NULL)
    {
        file_count++;
        i++;
    }

    char* line = NULL;
    size_t n = 0;
    for (i = optind+1; i < params_count; i++)
    {
        //open file as stream to use getline function
        int match_count = 0;
        FILE* file = fopen(params[i], "r");
        if (file == NULL)
        {
            perror("fopen");
            return -1;
        }
        ssize_t read_bytes;
        while ((read_bytes  = getline(&line, &n, file)) != -1)
        {
            regmatch_t pmatch[10];
            size_t nmatch = 5;
            printf("reading line\n");
            if (regexec(&pattern_regex, line, nmatch, pmatch, 0) == 0)
            {
                printf("regexec found match\n");
                if (flags.c)
                    match_count++;
                else
                {
                    if (file_count > 1)
                        printf("%s: ", params[i]);
                    printf("%s", line);
                }
            }
            if (flags.c)
            {
                if (file_count > 1)
                    printf("%s: ", params[i]);
                printf("%d", match_count);
            }
        }
        fclose(file);
        printf("\n");
    }
    return 0;
}
