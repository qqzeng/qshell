/**
@author: qqzeng
@email: qtozeng@gmail.com
@date: 17-11-13
@desc: 主函数
**/

#include "qshell.h"

#define TRUE 1

void process_command()
{
    pid_t pid;
    int status;
    char buf[MAXLINE];
    char prompt[MAX_PROMPT_LEN];

    while(TRUE)
    {
        type_prompt(prompt);
        // read_command(command,parameters);
        const char * command = "ls";
        int h = 32;
        char **parameters = (char **)malloc(sizeof(char*) * h);
        parameters[0] = "ls";
        parameters[1] = "-l";
        if ((pid = fork()) < 0) {
            err_sys("fork error!");
            exit(1);
        } else if (pid == 0) {
            /* child */
            execvp(command,parameters);
            err_ret("could't execute: %s ", buf);
            exit(127);
        }
        /* parent */
        if ((pid = waitpid(pid, &status, 0)) < 0){
            err_sys("waitpid error");
            printf("status: %d, pid: %d", status, pid);
        }
    }
}

int main() {

    process_command();
    return 0;
}
