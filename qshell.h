
#ifndef _QSHELL_H_
#define _QSHELL_H_

#endif //_QSHELL_H_
#include "apue.h"
#include <sys/wait.h>
#include <pwd.h>
#include "cmd_linklist.h"

#define MAX_PROMPT_LEN 1024
#define MAX_COMMAND_LEN 1024
#define MAX_SINGLE_CMD 64
#define MAXARG 20

// struct passwd* pwd;

void type_prompt(char*);
int read_command(char **, char **);

