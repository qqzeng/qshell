/**
@author: qqzeng
@email: qtozeng@gmail.com
@date: 17-11-13
@desc: 主函数
**/
#include "qshell.h"
#include "cmd_linklist.h"
#include "param_parse.h"
#include "builtin_command.h"
#include "init.h"
#include "command_execute.h"

#define TRUE 1

int process_command() {
	pid_t pid;
    char buf[MAXLINE];
    char prompt[MAX_PROMPT_LEN];
    int status;
    char **parameters = malloc(sizeof(char*)*(MAXARG + 2));
    char **command = malloc(sizeof(char*)*(MAXARG));
    int ncommand = 0;
    link_cmd_node *cmd_list;
    int npipe = 0;
	while(TRUE) 
	{
        printf("====qshell pid: %d=====\n",getpid());
        // show system prompt.
		type_prompt(prompt);
        // read command from stdio.
        ncommand = read_command(command,parameters);
        if(ncommand <= 0)
            continue;
        // parse command string array.
        cmd_list = cmd_parse(parameters, cmd_list, &npipe);
        // execute built-in command.
        if(builtin_cmd_process(cmd_list))
            continue;
        // execute commands.
        execute_cmd(cmd_list, npipe);
        printf("while exit.....");
    }
    return 1;
}

int main()
{
    init();
    process_command();
    return 0;
}