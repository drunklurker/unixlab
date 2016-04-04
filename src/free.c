#include "free.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>

int shcmd_free(char* cmd, char* params[])
{
    struct sysinfo info;

    int opt;
    char opts[] = "bkm";
    int param_count = 0;
    while (params[param_count] != NULL)
        param_count++;
    int divisor = 1;
    while ((opt=getopt(param_count, params, opts)) != -1)
    {
        switch (opt)
        {
        case 'b':
            divisor = 1;
            break;
        case 'k':
            divisor = 1024;
            break;
        case 'm':
            divisor = 1024*1024;
            break;
        }
    }

    int result = sysinfo(&info);

    if (result < 0)
        return result;

    printf("%d\n", info.mem_unit);

    printf("%-7s %10s %10s %10s %10s %10s\n", "", "total", "used", "free", "shared", "buffers");
    printf("%-7s %10lu %10lu %10lu %10lu %10lu\n", "Mem:", info.totalram/divisor, (info.totalram-info.freeram)/divisor,
            info.freeram/divisor, info.sharedram/divisor, info.bufferram/divisor);

    return result;
}
