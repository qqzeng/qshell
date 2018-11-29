/**
@author: qqzeng
@email: qtozeng@gmail.com
@date: 17-11-13
@desc: 主函数
**/
#include "qshell.h"
#define TRUE 1

int process_command() {
	pid_t pid;
    char buf[MAXLINE];
    char prompt[MAX_PROMPT_LEN];
    int status;
    char **parameters = malloc(sizeof(char*)*(MAXARG + 2));
    char **command = malloc(sizeof(char*)*(MAXARG));
    int ncommand = 0;
    link_cmd_node *cmd_list = NULL;
	while(TRUE) 
	{
        // show system prompt
		type_prompt(prompt);
        // read command from stdio
        ncommand = read_command(command,parameters);
        if(-1 == ncommand)
            continue
        param_parse(parameters,cmd_list);
        if(builtin_command(command,parameters))
            continue;
        continue;
		if ((pid = fork()) < 0) {
        	err_sys("fork error!");
        	exit(1);
        } else if (pid == 0) {
        	/* child */
			execvp(command[0], parameters);
        	err_ret("could't execute: %s ", buf);
    		exit(127);
        }
        /* parent */
        if ((pid = waitpid(pid, &status, 0)) < 0){
       		err_sys("waitpid error");
        	printf("status: %d, pid: %d", status, pid);
        }
	}

    return 1;
}


int main()
{
    process_command();
    return 0;
}