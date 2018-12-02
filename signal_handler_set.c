#include "qshell.h"
#include "init.h"
#include "cmd_linklist.h"
#include "builtin_command.h"
#include "qutil.h"
#include "signal_handler_set.h"

int handle_sigchild(int pid)
{
	process_node *p = process_head_node->next;
    process_node *pre = process_head_node;
    if (NULL == p) {printf("[qshell:info:handle_sigchild] empty bg linklist!\n");return 0;}
    while(p != NULL)
    {
    	if (p->npid == pid) {
        	pre->next = p->next;
            free(p);
            return 1;
        }
        pre = p;
        p = p->next;
    }
    return 0;
}

/* handle signal that the child process often bg exit */
void sig_handle_bgp_exit(int signo)
{
	if (signo == SIGCHLD) {printf("=====SIGCHLD====\n");}
	
	pid_t pid;
	int status;
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);
	while((pid = waitpid(-1, &status, 0)) > 0) {
		printf("[qshell:info:sig_handle_bg_exit] bgp pid=%d stopped!\n", pid);
		sigprocmask(SIG_BLOCK, &mask, NULL);
		if (handle_sigchild(pid)){
			printf("[qshell:info:sig_handle_bg_exit] removed bgp pid=%d !\n", pid);
		}
		sigprocmask(SIG_UNBLOCK, &mask, NULL);
	}
	return ;
}

