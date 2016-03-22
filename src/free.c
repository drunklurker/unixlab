#include "free.h"

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <sys/stat.h>
#include <sys/sysinfo.h>

int shcmd_free(char* cmd, char* params[])
{
    struct sysinfo info;

    int result = sysinfo(&info);

    if (result < 0)
        return result;

    printf("%d\n", info.mem_unit);

    printf("%-7s %10s %10s %10s %10s %10s\n", "", "total", "used", "free", "shared", "buffers");
    printf("%-7s %10lu %10lu %10lu %10lu %10lu\n", "Mem:", info.totalram/1024, (info.totalram-info.freeram)/1024,
            info.freeram/1024, info.sharedram/1024, info.bufferram/1024);

    return result;
}
