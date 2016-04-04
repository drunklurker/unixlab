#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <regex.h>
#include <string.h>
#include <malloc.h>

#include "rm.h"
#include "mkdir.h"
#include "free.h"
#include "rmdir.h"
#include "tail.h"
#include "cat.h"
#include "nl.h"
#include "link.h"
#include "wc.h"
#include "ls.h"
#include "grep.h"
#include "ps.h"

//cat wc nl link grep
int shell_active = 1; // требуется для команды exit

//  команды оформлены в виде макросов
//  доп. информация http://en.wikipedia.org/wiki/C_preprocessor

#define SHCMD(x) int shcmd_##x (char* cmd, char* params[])
#define SHCMD_EXEC(x) shcmd_##x (params[0], params)
#define IS_CMD(x) strncmp(#x,cmd,strlen(cmd)) == 0


char* paste_env(char *str)
{
    char *env;
    char buf[256];
    regex_t preg;
    regmatch_t pm;
    char *newstr;
    int len;

    regcomp( &preg, "\\$\\w\\+\\b", REG_ICASE );
    while ( regexec(&preg, str, 1, &pm, REG_NOTBOL) == 0 )
    {
	memset(buf, '\0', 256);
	strncpy(buf,&str[pm.rm_so]+1,(pm.rm_eo-pm.rm_so-1 < 256)?pm.rm_eo-pm.rm_so-1:254);
	env = getenv(buf);
	if(!env) env = "";
	len = strlen(str)-strlen(buf)+strlen(env);
	newstr=malloc(len);
	memset(newstr, '\0', len);
	strncpy(newstr,str,pm.rm_so);
	strncat(newstr,env,strlen(env));
	strncat(newstr,&str[pm.rm_eo],strlen(str)-pm.rm_eo);
	str = newstr;
    }
    regfree(&preg);

    return newstr;
}
// Встроенные команды
// - pwd, exit

SHCMD(pwd)
{
    printf("%s\n",getenv("PWD"));
    return 0;
}

SHCMD(exit)
{
    shell_active = 0;
    printf("Bye, bye!\n");
    return 0;
}


// выполнение команды с параметрами
void my_exec(char *cmd)
{
    char *params[256]; //параметры команды разделенные пробелами
    char *token;
    int np;
    token = strtok(cmd, " ");
    np = 0;
    while( token && np < 255 )
    {
        params[np++] = token;
        token = strtok(NULL, " ");
    }
    params[np] = NULL;

    // выполнение встроенных команд
    if( IS_CMD(pwd) )
        SHCMD_EXEC(pwd);
    else if ( IS_CMD(rm) )
        SHCMD_EXEC(rm);
    else if ( IS_CMD(mkdir) )
        SHCMD_EXEC(mkdir);
    else if ( IS_CMD(rmdir) )
        SHCMD_EXEC(rmdir);
    else if ( IS_CMD(free) )
        SHCMD_EXEC(free);
    else if ( IS_CMD(tail) )
        SHCMD_EXEC(tail);
    else if ( IS_CMD(cat) )
        SHCMD_EXEC(cat);
    else if ( IS_CMD(ls) )
        SHCMD_EXEC(ls);
    else if ( IS_CMD(nl) )
        SHCMD_EXEC(nl);
    else if ( IS_CMD(link) )
        SHCMD_EXEC(link);
    else if ( IS_CMD(wc) )
        SHCMD_EXEC(wc);
    else if ( IS_CMD(grep) )
        SHCMD_EXEC(grep);
    else if ( IS_CMD(ps) )
        SHCMD_EXEC(ps);
    else if( IS_CMD(exit) )
        SHCMD_EXEC(exit);
    else
    {
        // иначе вызов внешней команды
        execvp(params[0], params);
        perror("exec"); // если возникла ошибка при запуске
    }
}

// рекурсивная функция обработки конвейера
// параметры: строка команды, количество команд в конвейере, текущая (начинается с 0 )
int exec_conv(char *cmds[], int n, int curr)
{
    int fd[2],i;
    if( pipe(fd) < 0 )
    {
        printf("Cannot create pipe\n");
        return 1;
    }

    if( n > 1 && curr < n - 2 )
    {
        // first n-2 cmds
        if( fork() == 0 )
        {
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            my_exec(cmds[curr]);
            exit(0);
        }
        if( fork() == 0 )
        {
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);
            exec_conv(cmds,n,++curr);
            exit(0);
        }
    }
    else
    {
        // 2 last cmds or if only 1 cmd
        if( n == 1 && strcmp(cmds[0],"exit") == 0 )
        {
            // для случая команды exit обходимся без fork, иначе не сможем завершиться
            close(fd[0]);
            close(fd[1]);
            my_exec(cmds[curr]);
            return 0;
        }
        if( fork() == 0 )
        {
            if( n > 1 ) // if more then 1 cmd
                dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            my_exec(cmds[curr]);
            exit(0);
        }
        if( n > 1 && fork()==0 )
        {
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);
            my_exec(cmds[curr+1]);
            exit(0);
        }
    }
    close(fd[0]);
    close(fd[1]);

    for( i = 0 ; i < n ; i ++ ) // ожидание завершения всех дочерних процессов
        wait(0);
    return 0;
}
// главная функция, цикл ввода строк (разбивка конвейера, запуск команды)
int main()
{
    char cmdline[1024];// = malloc(1024*sizeof(cmdline[0]));
    char *p, *cmds[256], *token;
    int cmd_cnt;

    while( shell_active )
    {
        printf("[%s]# ",getenv("PWD"));
        fflush(stdout);

        fgets(cmdline,1024,stdin);
        paste_env(cmdline);
        if( (p = strstr(cmdline,"\n")) != NULL )
            p[0] = 0;

        token = strtok(cmdline, "|");
        for(cmd_cnt = 0; token && cmd_cnt < 256; cmd_cnt++ )
        {
            cmds[cmd_cnt] = strdup(token);
            token = strtok(NULL, "|");
        }
        cmds[cmd_cnt] = NULL;

        if( cmd_cnt > 0 )
        {
            exec_conv(cmds,cmd_cnt,0);
        }
    }
    return 0;
}
