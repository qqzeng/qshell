/**
@author: qqzeng
@email: qtozeng@gmail.com
@date: 17-11-13
@desc: 主函数
**/

#include "unistd.h"
#include <stdlib.h>
#include <stdio.h>
#include <error.c>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <math.h>

#define BUF_SIZE 100
#define TRUE 1
#define MAX_PROMPT_LEN 1024

void process_command()
{
    pid_t pid;
    int status;
    char buf[MAXLINE];
    char prompt[MAX_PROMPT_LEN];

    while(TRUE)
    {
        // type_prompt(prompt);
        // read_command(command,parameters);
        const char * command = "wc";
        int h = 32;
        char **parameters = (char **)malloc(sizeof(char*) * h);
        parameters[0] = "wc";
        parameters[1] = "pipe_command_test.c";
        parameters[2] = ">";
        parameters[3] = "tmp.txt";
        parameters[4] = "2>&1";
        parameters[5] = NULL;
        printf("=====%s\n", parameters);
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
        break;
    }
}

void test_str(){
    size_t offset = strcspn("abcdefg", "d");
    printf("%d\n", offset);
}

void test_chdir()
{
    errno = 0;
    char* curr_working_path = (char*)malloc((sizeof(char))*100);
    // int res = chdir("~");
    // getcwd(curr_working_path, 100);
    // printf("status: %d %s\n", res, curr_working_path);

    int res = chdir("/home");
    getcwd(curr_working_path, 100);
    printf("status: %d %s\n", res, curr_working_path);
    chdir("/home/qqzeng");
    getcwd(curr_working_path, 100);
    printf("status: %d %s\n", res, curr_working_path);
    chdir("/root");

    getcwd(curr_working_path, 100);
    printf("status: %d %s\n", res, curr_working_path);
    chdir("~/workspace/c/operating_system");
    getcwd(curr_working_path, 100);
    printf("status: %d %s\n", res, curr_working_path);

    chdir("..");
    getcwd(curr_working_path, 100);
    printf("status: %d %s\n", res, curr_working_path);
            if (0 != errno){
        printf("===errno:%d\t", errno);
        err_sys("");
    }
    chdir(".");
    getcwd(curr_working_path, 100);
    printf("status: %d %s\n", res, curr_working_path);

    chdir("");
    getcwd(curr_working_path, 100);
    printf("status: %d %s\n", res, curr_working_path);

}

void test_pointer()
{
    char** a = malloc(sizeof(char*)*10);
    a[0] = "abc";
    a[1] = "abc";
    a[2] = "bcd";
    a[3] = "cdf";
    a[4] = "defg";
    a[5] = NULL;
    char** p = a;
    while (*p != NULL) {
        printf("%s\t", *p);
        p++;
    }
}

void test_value_pass(int *b)
{
    int i = 10;
    *b = i;
}

void test_str_cpy(){
    char *t = malloc(sizeof(32));
    char p[] = "abcd";
    strcpy(t, p);
    char** arg = malloc(sizeof(char*)*100);
    arg[0] = t;
    printf("%s\t\n", arg[0]);
}

void test_pointer2()
{
    char** arguments = malloc(sizeof(char*)*10);
    arguments[0] = "ls";
    arguments[1] = "-l";
    arguments[2] = "&";
    arguments[3] = NULL;
    char** arg_p = arguments;
    // for (int i = 0; *arg_p != NULL; arg_p++) {
    //     printf("%s\t", *arg_p);
    // }
    while (*(arg_p+1)) { printf("%s\n", *arg_p);arg_p++;}
    printf("----------------\n");
    arg_p = NULL;
    for (int i = 0; arg_p != NULL; i++) {
        printf("%s\t", *arg_p);
    }
}
/* int to str */
static int int_to_str(int num, char* str, int base)
{
    // printf("=====%d\t%d\t%s\n", num, base, num);
    if (NULL == str){return 0;}
    switch(base)
        {
        case 10:
            sprintf(str, "%d", num);
    // printf("===%s\n", str);
            break;
        case 16:
            sprintf(str, "%x", num);
            break;
        case 8:
            sprintf(str, "%o", num);
            break;
        default:
            printf("[qshell:error:int_to_str] unsupported base value!\n");
            return 0;
    }
        
    return 1;
}
void test_proc_status()
{
    int pid = 6941;
    char file_path[4096] = "/proc/";
    char pid_str[(int)(log10(32768) + 1)];
    int_to_str(pid, pid_str, 10);
    printf("%s\n", pid_str);
    char* str2 = "/status";
    strcat(file_path, pid_str);
    strcat(file_path, str2);
    printf("%s\n", file_path);
    FILE *fp = fopen(file_path, "r");
    if (!fp) {printf("[qshell:error:get_process_status] file open error!\n");return ;}
    int i = 0;
    if (fp) {
        while (i < 3) {
            char buf[100];
            fgets(buf, 102, fp);
            char *delim = "(";
            char *p;
            printf("%s\n", strtok(buf, delim));
             while((p = strtok(NULL, delim)))
                printf("==%s==\n", p);
            if (i == 2){
                if (p[0] == 'D' || p[0] == 'X') {
                    return ;
                }
            }
            i++;
        }
    } else {printf("[qshell:error:other_init] file open error!\n");}
}

void test_int_value()
{
    printf("%d\n", (1 << sizeof(int) * 8 - 1) - 1);
}

void test_max_pid()
{
    int fd = open("/proc/sys/kernel/pid_max", O_RDONLY);
    if (fd) {
        char buf[10];
        read(fd, buf, 10);
        printf("%s\n", buf);
        int iii = atoi(buf);
        printf("%d\n", iii);
    }

    char *a;
    printf("%s===", a);
}

void sleep_1()
{
    for (int i = 0; i < 1000000000; i++) {
        printf("====%d\n", i);
        // sleep(1);
    }
}

void test_stroke()
{
    char s[18] = "State: (sleeping)";
    char *d = "(";
    char *t;
    t = strtok(s, d);
    printf("====%s====\n", t);
    if (!strncmp(t, "State", 5)) {
        while(t != NULL){  
            printf("%s\n", t);  
            t = strtok(NULL, d);  
        }
    }
}
int main() {

    // test_str();
    // test_chdir();
    // process_command();
    // test_pointer();

    // int a = 1;
    // test_value_pass(&a);
    // printf("%d\n", a);

    // test_str_cpy();
    // test_pointer2();
    // test_proc_status();
    // test_max_pid();
    // sleep_1();
    test_stroke();
    return 0;
}
