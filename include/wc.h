#ifndef WC_H
#define WC_H

#include <stdlib.h>

struct wc_struct
{
    int L;
    int l;
    int w;
    int c;
};
int count_line_words(char* line, size_t length, struct wc_struct* flags);
int shcmd_wc(char* cmd, char* params[]);

#endif // WC_H
