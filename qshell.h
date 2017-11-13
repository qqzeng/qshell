#ifndef QSHELL_QSHELL_H
#define QSHELL_QSHELL_H

#endif

#include "apue.h"
#include <sys/wait.h>
#include <pwd.h>

#define MAX_PROMPT_LEN 1024

void type_prompt(char*);

int read_command(char **command, char **parameters);