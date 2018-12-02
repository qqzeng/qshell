#include "qshell.h"
#include "cmd_linklist.h"
#include "command_execute.h"
#include "param_parse.h"
#include "init.h"
#include "signal_handler_set.h"

// #define DEBUG

struct sigaction act_cmd_execute;

void sig_handle_bgp_exit_2(int signo)
{
    if (signo == SIGCHLD) {printf("=====SIGCHLD====\n");}
    int status;
    wait(&status);
    printf("=========status=%d=======\n", status);
    if(WIFEXITED(status)) {
        printf("SIGCHLD--->%d\n",WEXITSTATUS(status));
    }
}

/* actions before command executing */
void setup()
{
    if (signal(SIGCHLD, sig_handle_bgp_exit) == SIG_ERR) {
        printf("signal installs error!\n");
    }
}

void execute_cmd(link_cmd_node* cmd_node, int npipe)
{
    // setup();
    link_cmd_node* p_node = cmd_node;
#ifdef DEBUG
    printf("npipe=%d\n", npipe);
#endif
    cmd_node = cmd_node->next;
    struct cmd_info *command = &(cmd_node->info_data);
    int status;
    int i = 0;
    pid_t pid;
    int pipe_fds[2*npipe]; 
    int j = 0;

    for(i = 0; i < (npipe); i++){
        if(pipe(pipe_fds + i*2) < 0) {
            printf("pipe error");
            exit(EXIT_FAILURE);
        }
    }
    while(cmd_node) {
#ifdef DEBUG
        printf("[qshell:info:execute_cmd] %s\n", command->cmd);
        printf("======j: %d======\n", j);
#endif
        // bg process
        // won't call <wait> to wait child to exit.
        // parent will ignore SIGCHLD signal to avoid zombie process.
        if ((command->flag & BACKGROUND)) {
            signal(SIGCHLD, sig_handle_bgp_exit_2);
            // signal(SIGCHLD, SIG_IGN);
            // remove the redundant argument info. 
            // remove the last character, i.e. sysmbol '&'.
            char** arg_p = command->arguments;
            while (*(arg_p+1)) { /*printf("%s\n", *arg_p);*/arg_p++;}
            *arg_p = NULL;
        } else {
            signal(SIGCHLD, SIG_DFL);
        }
        pid = fork();
        if (pid == 0) {

            //[redirect] in redirect
            if(command->flag & IN_REDIRECT) {
            #ifdef DEBUG
                printf("========in_redirect========\n");
            #endif
                if (in_redirect_info_parse(command)){
                    close(STDIN_FILENO);
                    dup2(command->in_fd, fileno(stdin)); // TODO: check duplicate result carefully.
                } else {
                    perror("[1] in redirect error");
                    exit(EXIT_FAILURE);
                }
            }

            //[redirect] out redirect, except (cat < qshell.c >> tmp >> tmp2)
            if ((command->flag & OUT_REDIRECT) || (command->flag & OUT_REDIRECT_APPEND)) {
            #ifdef DEBUG
                printf("========out_redirect========\n");
            #endif
                if (out_redirect_info_parse(command)) {
                    close(STDOUT_FILENO);
                    dup2(command->out_fd, STDOUT_FILENO); // TODO: check duplicate result carefully.
                } else {
                    perror("[2] out redirect error");
                    exit(EXIT_FAILURE);
                }
            }

            // cmd_linklist_print(p_node);

            //[pipe] if not last command, out redirect
            if (cmd_node->next && command->out_fd == STDOUT_FILENO) {
                if (dup2(pipe_fds[j + 1], STDOUT_FILENO) < 0) {
                    perror("[1] dup2 error");
                    exit(EXIT_FAILURE);
                }
            }
            //[pipe] if not first command && j!= 2*npipe, in redirect
            if (j != 0 && command->in_fd == STDIN_FILENO) {
                if (dup2(pipe_fds[j - 2], STDIN_FILENO) < 0) {
                    perror("[2] dup2 error");///j-2 0 j+1 1
                    exit(EXIT_FAILURE);
                }
            }
            for (i = 0; i < 2*npipe; i++) {
                close(pipe_fds[i]);
            }

            // let bg process ignore int&quit signal
            if (!(command->flag & BACKGROUND)) {
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);
            } else {
                signal(SIGINT, SIG_IGN);
                signal(SIGQUIT, SIG_IGN);
            }

            // command_run(command);
            if (execvp(*command->arguments, command->arguments) < 0) {
                perror(*command->arguments);
                exit(EXIT_FAILURE);
            }
            // close in&out redirect fd. it doesn't work.
            // close(command->in_fd);
            // close(command->out_fd);
        } else if(pid < 0) {
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        if ((command->flag & BACKGROUND)){
            // add to process linklist
            process_node *p=(process_node*)malloc(sizeof(process_node));
            p->npid = pid;
            p->cmd = command->cmd;
            p->arg = command->arguments;
            p->next = process_head_node->next;
            process_head_node->next=p;
            printf("pid:%d\n", p->npid);
        }
        cmd_node = cmd_node->next;
        if (!cmd_node) break;
        command = &(cmd_node->info_data);
        j+=2;
    }
    /*Parent closes the pipes*/
    for(i = 0; i < 2 * npipe; i++){
        close(pipe_fds[i]);
    }
    // wait
    if (!(command->flag & BACKGROUND)) {
        for(i = 0; i < npipe + 1; i++){
            wait(&status);
            printf("===========status=%d\n", status);
        }
    }
}