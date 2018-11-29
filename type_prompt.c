/**
@author: qqzeng
@email: qtozeng@gmail.com
@date: 17-11-13
@desc: 模拟linux shell提示符输出
**/

#include "qshell.h"
// #define DEBUG

const int MAX_NAME_LEN = 256;
const int MAX_PATH_LEN = 1024;


void change_color(char *prompt)
{
    // change color of font
    char* color_head = "\033[01;33m";
    char* color_end = "\033[0m";
    char* prompt_mix = malloc(sizeof(char)*(MAX_PATH_LEN + MAX_NAME_LEN + 1));
    sprintf(prompt_mix, "%s", color_head);
    int length2 = strlen(prompt_mix);
    sprintf(prompt_mix + length2, "%s", prompt);
    length2 = strlen(prompt_mix);
    sprintf(prompt_mix + length2, "%s", color_end);
    printf("%s ", prompt_mix);
    return;
}

void type_prompt(char *prompt)
{
    struct passwd *pwd = getpwuid(getuid()) ;
    char hostname[MAX_NAME_LEN];
    char curr_working_path[MAX_PATH_LEN];
    int length;
    int host_len;
    pwd = getpwuid(getuid());
    getcwd(curr_working_path, MAX_NAME_LEN);
    if(0 == gethostname(hostname, MAX_PATH_LEN))
        sprintf(prompt, "[qshell]%s@%s:", pwd->pw_name,hostname);
    else
        sprintf(prompt, "[qshell]%s@unknown:", pwd->pw_name);
    length = strlen(prompt);
    host_len = length;
    if(strlen(curr_working_path) < strlen(pwd->pw_dir) ||
       0 != strncmp(curr_working_path, pwd->pw_dir, strlen(pwd->pw_dir)))
        sprintf(prompt+length, "%s", curr_working_path);
    else
        sprintf(prompt+length,"~%s", curr_working_path + strlen(pwd->pw_dir));
    length = strlen(prompt);
    if(geteuid() == 0)
        sprintf(prompt + length, "#");
    else
        sprintf(prompt + length, "$");
    #ifdef DEBUG
        printf("[qshell:info] prompt=%s\n", prompt);
    #endif
    change_color(prompt);
    return;
}



