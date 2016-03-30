#ifndef TAIL_H
#define TAIL_H

#include <stdio.h>

struct tail_struct
{
    int n;
    int last;
};

int shcmd_tail(char* cmd, char* params[]);
int print_last_lines(FILE* file, int line_count, char* path);
int print_lines_starting_with(FILE* file, int lines_to_skip, char* path);

#endif // TAIL_H
