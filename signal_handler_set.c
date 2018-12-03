#include "qshell.h"
#include "cmd_linklist.h"
#include "builtin_command.h"
#include "qutil.h"
#include "signal_handler_set.h"

/* handle signal that the child process often bg exit */
void sig_handle_bgp_exit(int signo)
{
	pid_t pid;
	int status;
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);
	while((pid = waitpid(-1, &status, 0)) > 0) {
		printf("[qshell:info:sig_handle_bg_exit] bgp pid=%d stopped!\n", pid);
		sigprocmask(SIG_BLOCK, &mask, NULL);
		if (remove_process_by_id(pid)){
			printf("[qshell:info:sig_handle_bg_exit] removed bgp pid=%d !\n", pid);
		}
		sigprocmask(SIG_UNBLOCK, &mask, NULL);
	}
	return ;
}

