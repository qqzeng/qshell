#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void catchsig(int sig)
{
    printf("i am %d signal\n", sig);
}

int main()
{
    signal(SIGCHLD, catchsig);
    pid_t id = fork();
    if(id == 0)
    {
        printf("i am child ,quit!pid:%d\n", getpid());
        exit(1);
    }
    else
    {
        // while(1)
        // {
            printf("i am father,runing!pid:%d\n", getpid());
            sleep(1);
        // }
    }
    return 0;
}