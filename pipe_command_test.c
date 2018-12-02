/**
@author: qqzeng
@email: qtozeng@gmail.com
@date: 17-12-09
@desc: 内建命令的执行
**/
#include "qshell.h"
#include <pwd.h>
#include "errno.h"
#include "init.h"
#include "cmd_linklist.h"
#include "builtin_command.h"

// #define DEBUG

static builtin_cmd builtins[] = 
{
    {"exit", do_exit},
    {"cd", do_cd},
    {"jobs",do_jobs},
    {"kill",do_kill},
    {NULL, NULL}
};


// we handle the built-in cmd easily first, that is to say, we won't take the situation into consideration
// which are similiar to <kill -9 pidNum> | <jobs &> etc.
int check_builtin_cmd(char* cmd_name, link_cmd_node *cmd_node)
{
    return find_specific_cmd(cmd_name, cmd_node);
}

/* only handle built-in command like exit, quit, cd, kill */
int builtin_cmd_process(link_cmd_node *cmd_node)
{
    int i = 0;
    int found = 0;
    while (builtins[i].cmd_name != NULL)
    {
        if (check_builtin_cmd(builtins[i].cmd_name, cmd_node))
        {
            builtins[i].handler(builtins[i].cmd_name, cmd_node);
        #ifdef DEBUG
            printf("[qshell:info:built-in_command] %s\n", builtins[i].cmd_name);
        #endif
            found = 1;
            break;
        }
        i++;
    }
    return found;
}

int do_exit(char* cmd_name, link_cmd_node* cmd_node)
{
    #ifdef DEBUG
       printf("[qshell:info:built-in_command] %s\n", cmd_name);
    #endif
    int n_bg_process = 0;
    process_node* p = process_head_node->next;
    while(p != NULL) {
        n_bg_process++;
        p = p->next;
    }
    if(0 != n_bg_process){
    #ifdef DEBUG
       printf("[qshell:info:built-in_command] some process are running in the background,are you sure to exit?y/N\n");
    #endif
       char exit_code = getchar();
       if(exit_code == 'N')
           return 1;
    } 
    printf("exit... \n");
    exit(EXIT_SUCCESS);
}

int do_jobs(char* cmd_name, link_cmd_node* cmd_node)
{
    process_node *p = process_head_node->next;
    if (NULL == p) {
        printf("[qshell:info:built-in_command] there is no bg process.\n");
        return 1;
    }
    while(NULL != p) {
        int i = 0;
        printf("%d\t%s\t",p->npid, p->cmd);
        char** arg_p = p->arg;
        while (*arg_p) {
            printf("%s ", *arg_p++);
        }
        printf("&\n");
        p = p -> next;
    }
    return 1;
}

// kill pidnum
int do_kill(char* cmd_name, link_cmd_node* cmd_node)
{
    if (NULL == cmd_node || NULL == cmd_node->next) {
        perror("[qshell:info:built-in_command] empty command linklist.");
        return 0;
    }
    int pidnum = atoi(cmd_node->next->info_data.arguments[1]);
    signal(SIGQUIT, SIG_DFL);
    kill(pidnum, SIGQUIT);
    signal(SIGQUIT, SIG_IGN);
    process_node *p = process_head_node->next;
    process_node *pre = process_head_node;
    while(p != NULL)
    {
        if(p->npid == pidnum){
            pre->next = p->next;
            free(p);
            return 1;
        }
        pre = p;
        p = p->next;
    }
    printf("[qshell:info:built-in_command] no pidnum = %d bg process\n", pidnum);
    return 0;
}

int do_cd(char* cmd_name, link_cmd_node* cmd_node)
{
    if (NULL == cmd_node || NULL == cmd_node->next) {
        perror("[qshell:info:built-in_command] empty command linklist.");
        return 0;
    }
    char** parameters = cmd_node->next->info_data.arguments;

    char *cd_path = NULL;
    struct passwd *pwd = getpwuid(getuid());
    // NULL, ~ or starts with ~ 
    if (NULL == parameters[1] || 0 == strcmp(parameters[1],"~") || parameters[1][0] == '~'){
        if (NULL == parameters[1]) {
            cd_path = (char*) malloc(strlen(pwd -> pw_dir));
        } else {
            cd_path = (char*) malloc(strlen(pwd -> pw_dir)+ strlen(parameters[1]));
        }
        strcpy(cd_path,pwd->pw_dir);
        if (NULL != parameters[1]) {
            strncpy(cd_path+strlen(pwd->pw_dir),parameters[1]+1,strlen(parameters[1]));
        }
    } else {
        cd_path = malloc(strlen(parameters[1]+1));
        strcpy(cd_path,parameters[1]);
    }
#ifdef DEBUG
    printf("[qshell:info:built-in_command] %s\t%s\n", parameters[0], parameters[1]);
    printf("cd_path:%s\n", cd_path);
#endif
    if (0 != chdir(cd_path)){
        printf("[qshell:info:built-in_command]: cd: %s:%s\n",cd_path,strerror(errno));
        return 0;
    }
    return 1;
#ifdef DEBUG
        printf("cd_path:%s\n", cd_path);
        int res = chdir(cd_path);
        char* curr_working_path = (char*)malloc((sizeof(char))*100);
        getcwd(curr_working_path, 100);
        printf("status: %d %s\n", res, curr_working_path);
#endif
        free(cd_path);
}


int builtin_command(char *command, char **parameters)
{
    struct passwd *pwd = getpwuid(getuid());
    if(0 == strcmp(command,"exit") || 0 == strcmp(command,"quit"))
        exit(0);
    else if(0 == strcmp(command,"about"))
    {
        printf("This is a simulation of shell (bash) in Linux.\n");
        return 1;
    }
    //TODO: regex match like /bin/echo
    else if(0 == strcmp(command,"cd"))
    {
        char *cd_path = NULL;
        // NULL, ~ or starts with ~ 
        if (parameters[1] == NULL || 0 == strcmp(parameters[1],"~") || parameters[1][0] == '~'){
            cd_path = (char*) malloc(strlen(pwd->pw_dir)+strlen(parameters[1]));
            strcpy(cd_path,pwd->pw_dir);
            strncpy(cd_path+strlen(pwd->pw_dir),parameters[1]+1,strlen(parameters[1]));
        } else {
            cd_path = (char*) malloc(strlen(parameters[1]+1));
            strcpy(cd_path,parameters[1]);
        }
        if (0 != chdir(cd_path)){
            printf("[qshell]: cd: %s:%s\n",cd_path,strerror(errno));
        }
#ifdef DEBUG
        printf("cd_path:%s\n", cd_path);
        int res = chdir(cd_path);
        char* curr_working_path = (char*)malloc((sizeof(char))*100);
        getcwd(curr_working_path, 100);
        printf("status: %d %s\n", res, curr_working_path);
#endif
        free(cd_path);
    }
    return 0;
}


// int main()
// {
//     char **parameters = malloc(sizeof(char*)*(100 + 2));
//     parameters[0] = "cd";
//     parameters[1] = "~/workspace/c";
//     parameters[2] = NULL;
//  builtin_command("cd", parameters);
//  return 0;
// }