#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// void new_op(int,siginfo_t*,void*);

// int main(int argc,char**argv)
// {
// 	struct sigaction act;
// 	int i;
// 	pid_t child_pid;
// 	sigemptyset(&act.sa_mask);
// 	act.sa_sigaction=new_op;//三参数信号处理函数
// 	act.sa_flags=SA_SIGINFO;//信息传递开关
// 	if(sigaction(SIGCHLD,&act,NULL) < 0)
// 	{
// 		printf("install sigal error\n");
// 	}
// 	while(1)
// 	{
// 		child_pid = fork();
// 		if (child_pid < 0) {printf("error!");}
// 		else if(child_pid == 0){
// 			sleep(2);
// 			exit(0);
// 		} else {
// 			sleep(3);
// 			printf("wait for the signal\n");
// 		}
// 	}
// }
// void new_op(int signum,siginfo_t *info,void *myact)//三参数信号处理函数的实现
// {
// 	pid_t pid;
// 	int status;
// 	sigset_t mask;
// 	sigemptyset(&mask);
// 	sigaddset(&mask, SIGCHLD);
// 	while((pid = waitpid(-1, &status, 0)) > 0) {
// 		printf("bgp pid=%d stopped!\n", pid);
// 		sigprocmask(SIG_BLOCK, &mask, NULL);
// 		printf("removed bgp pid=%d !\n", pid);
// 		sigprocmask(SIG_UNBLOCK, &mask, NULL);
// 	}
// 	printf("handle signal %d over;\n", signum);
// }

void new_op(int);

int main(int argc,char**argv)
{
	int i;
	pid_t child_pid;
	if(signal(SIGCHLD,new_op) == SIG_ERR)
	{
		printf("install sigal error\n");
	}
	while(1)
	{
		child_pid = fork();
		if (child_pid < 0) {printf("error!");}
		else if(child_pid == 0){
			sleep(2);
			// exit(0);
			execlp("ls", "ls", NULL);
		} else {
			sleep(3);
			printf("wait for the signal\n");

		}
	}
}
void new_op(int signum)//三参数信号处理函数的实现
{
	pid_t pid;
	int status;
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);
	while((pid = waitpid(-1, &status, 0)) > 0) {
		printf("=========%d=======\n", status);
		printf("bgp pid=%d stopped!\n", pid);
		sigprocmask(SIG_BLOCK, &mask, NULL);
		printf("removed bgp pid=%d !\n", pid);
		sigprocmask(SIG_UNBLOCK, &mask, NULL);
	}
	printf("handle signal %d over;\n", signum);
}