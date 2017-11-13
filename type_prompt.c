/**
@author: qqzeng
@email: qtozeng@gmail.com
@date: 17-11-13
@desc: 模拟linux shell提示符输出
**/

#include "qshell.h"

const int MAX_NAME_LEN = 256;
const int MAX_PATH_LEN = 1024;

void type_prompt(char *prompt)
{
    struct passwd *pwd;
    char *hostname;
    char *curr_working_path;
    int length;
    pwd = getpwuid(getuid());
    getcwd(curr_working_path, MAX_NAME_LEN);
    if(0 == gethostname(hostname,MAX_PATH_LEN))
        sprintf(prompt,"[qshell]%s@%s:",pwd->pw_name,hostname);
    else
        sprintf(prompt,"[qshell]%s@unknown:",pwd->pw_name);
    length = strlen(prompt);
    if(strlen(curr_working_path) < strlen(pwd->pw_dir) ||
       0 != strncmp(curr_working_path, pwd->pw_dir, strlen(pwd->pw_dir)))
        sprintf(prompt+length,"%s",curr_working_path);
    else
        sprintf(prompt+length,"~%s",curr_working_path+strlen(pwd->pw_dir));
    length = strlen(prompt);
    if(geteuid()==0)
        sprintf(prompt+length,"#");
    else
        sprintf(prompt+length,"$");
    return;
}



