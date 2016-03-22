#ifndef LS_H
#define LS_H

#include <dirent.h>

struct ls_struct
{
    int l;
    int a;
    int i;
};

int shcmd_ls(char* cmd, char* params[]);

int print_ls_l(const struct dirent* d);

int always_true(const struct dirent* d);

#endif // LS_H
