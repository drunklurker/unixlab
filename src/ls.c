#include "ls.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define LIST_LENGTH 4096

int always_true(const struct dirent *d)
{
    return 1;
}

int shcmd_ls(char* cmd, char* params[])
{
    int params_count = 0;
    while (params[params_count] != NULL)
        params_count++;
    printf("ls started\n");
    struct ls_struct flags;
    flags.l = flags.a = flags.i = 0;
    char opts[] = "lai";
    int opt;
    while ((opt = getopt(params_count, params, opts)) != -1)
    {
        switch(opt)
        {
        case 'l':
            flags.l = 1;
            break;
        case 'a':
            flags.a = 1;
            break;
        case 'i':
            flags.i = 1;
            break;
        }
    }
    char path[500];
    if (params[optind] == NULL)
        strcpy(path, getenv("PWD"));
    else
        strcpy(path, params[optind]);
    printf("optind = %d\n", optind);
    printf("path = %s\n", path);
    int i;
    for (i = 0; i < params_count; i++)
        printf("params[%d] = %s\n", i, params[i]);
    struct dirent ** entry_list;
    int entry_count = scandir(path, &entry_list, always_true, alphasort);
    int current_length = 0;
    int terminal_width = 80;
    for (i = 0; i < entry_count; i++)
    {
        if (flags.a == 0 && entry_list[i]->d_name[0] == '.')
            continue;
        if (flags.l)
            print_ls_l(entry_list[i], path, flags.i);
        else
        {
            char filename[1024];
            if (flags.i == 0)
                strcpy(filename, "");
            else
                sprintf(filename, "%d ", entry_list[i]->d_ino);
            strcat(filename, entry_list[i]->d_name);
            if (current_length + strlen(filename) + 1 >= terminal_width)
            {
                printf("\n");
                current_length = 0;
            }
            printf("%s  ", filename);
            current_length += strlen(filename) + 2;
        }
    }

    printf("\n");
    return 0;
}
int print_ls_l(const struct dirent* d, const char* path, int print_inode)
{
    char time[256];
    struct stat st;
    struct passwd *pwd;
    struct group *grp;
    int mode,right_ind;
    char* rights = "rwxrwxrwx";
    char *nuser,*ngroup;
    char fullname[1024];
    strcpy(fullname, path);
    if (fullname[strlen(fullname)] != '/')
        strcat(fullname, "/");
    strcat(fullname, d->d_name);
    lstat(fullname,&st);
	mode = st.st_mode;
	if (print_inode)
    {
        printf("%d ", d->d_ino);
    }
	S_ISDIR(mode)?printf("d"):printf("-"); // TODO: Fix me
	mode &= 0777;
	right_ind = 0;
	while( right_ind < 9 )
	{
	    mode&256?printf("%c",rights[right_ind]):printf("-");
	    mode <<= 1;
	    right_ind++;
	}
	if (pwd=getpwuid(st.st_uid))
        nuser=pwd->pw_name;
    else
        sprintf(nuser,"%d",st.st_uid);

	if (grp=getgrgid(st.st_gid))
        ngroup=grp->gr_name;
    else
        sprintf(ngroup,"%d",st.st_gid);

	strftime(time, sizeof(time), "%Y-%m-%d %H:%M", localtime(&st.st_mtime));

    if (print_inode != 0)
        printf("%3d %6s %6s %5d %s %s\n",st.st_nlink,nuser,ngroup,st.st_size, time, d->d_name);
    else
        printf("%3d %6s %6s %5d %s %s\n",st.st_nlink,nuser,ngroup,st.st_size, time, d->d_name);

    return 0;
}
