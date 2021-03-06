// FIFO的打开规则：
// 1. 如果当前打开操作是为读而打开FIFO时，若已经有相应进程为写而打开该FIFO，则当前打开操作将成功返回；
// 否则，可能阻塞直到有相应进程为写而打开该FIFO（当前打开操作设置了阻塞标志）；或者，成功返回（当前打开操作没有设置阻塞标志）。
// 2. 如果当前打开操作是为写而打开FIFO时，如果已经有相应进程为读而打开该FIFO，则当前打开操作将成功返回；
// 否则，可能阻塞直到有相应进程为读而打开该FIFO（当前打开操作设置了阻塞标志）；或者，返回ENXIO错误（当前打开操作没有设置阻塞标志）。


#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#define FIFO_SERVER "/tmp/fifoserver"

int handle_client(char*);
int w_open(char*arg);

void main(int argc,char** argv)
{
	int r_rd;
	int w_fd;
	pid_t pid;
	if((mkfifo(FIFO_SERVER,O_CREAT|O_EXCL) < 0) && (errno != EEXIST))
		printf("cannot create fifoserver\n");
	handle_client(FIFO_SERVER);
}
int handle_client(char* arg)
{
	int ret;
	ret = w_open(arg);
	switch(ret)
	{
		case 0:
		{
			printf("open %s error\n",arg);
			printf("no process has the fifo open for reading\n");
			return -1;
		}
		case -1:
		{
			printf("something wrong with open the fifo except for ENXIO");
			return -1;
		}
		case 1:
		{
			printf("open server ok\n");
			return 1;
		}
		default:
		{
			printf("w_no_r return ----\n");
			return 0;
		}
	}
	unlink(FIFO_SERVER);
}

//0  open error for no reading
//-1 open error for other reasons
//1  open ok
int w_open(char*arg)
{
	if(open(arg, O_WRONLY|O_NONBLOCK, 0) == -1) // open for write and non-block
	{
		if(errno == ENXIO)
		{
			return 0;
		}
		else
			return -1;
	}
	return 1;
}