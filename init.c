#include "qshell.h"
#include "init.h"
#include "cmd_linklist.h"
#include "fcntl.h"


void init()
{
	init_bgp_process_linklist();
	singal_init();
	other_init();
}

/* other value init*/
void other_init()
{
	PID_VALUE_PATH = "/proc/sys/kernel/pid_max";
	int fd = open("/proc/sys/kernel/pid_max", O_RDONLY);
    if (fd) {
        char buf[10];
        read(fd, buf, 10);
        PID_MAX_VALUE = atoi(buf);
    } else {printf("[qshell:error:other_init] file open error!\n");}
}

// init the bg process linklist
void init_bgp_process_linklist()
{
	process_head_node = (process_node*)malloc(sizeof(process_node));
    process_head_node -> next = NULL;
}

// handle signal of "CTRL+C"
void singal_init()
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void sigint_handler(int sig)
{
	// do nothing...
	fflush(stdout);
}