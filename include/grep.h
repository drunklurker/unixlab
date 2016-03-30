#ifndef grep_h
#define grep_h

#include <stdlib.h>

struct grep_struct
{
    int c;
};

int shcmd_grep(char *cmd, char* params[]);

#endif
