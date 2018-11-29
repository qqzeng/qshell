/*
从FIFO中读取数据：
约定：如果一个进程为了从FIFO中读取数据而阻塞打开FIFO，那么称该进程内的读操作为设置了阻塞标志的读操作。
1. 如果有进程写打开FIFO，且当前FIFO内没有数据，则对于设置了阻塞标志的读操作来说，将一直阻塞。
对于没有设置阻塞标志读操作来说则返回-1，当前errno值为EAGAIN，提醒以后再试。
2. 对于设置了阻塞标志的读操作说，造成阻塞的原因有两种：当前FIFO内有数据，但有其它进程在读这些数据；
另外就是FIFO内没有数据。解阻塞的原因则是FIFO中有新的数据写入，不论信写入数据量的大小，也不论读操作请求多少数据量。
3. 读打开的阻塞标志只对本进程第一个读操作施加作用，如果本进程内有多个读操作序列，则在第一个读操作被唤醒并完成读操作后，其它将要执行的读操作将不再阻塞，即使在执行读操作时，FIFO中没有数据也一样（此时，读操作返回0）。
4. 如果没有进程写打开FIFO，则设置了阻塞标志的读操作会阻塞。
注：如果FIFO中有数据，则设置了阻塞标志的读操作不会因为FIFO中的字节数小于请求读的字节数而阻塞，此时，读操作会返回FIFO中现有的数据量。

向FIFO中写入数据：
约定：如果一个进程为了向FIFO中写入数据而阻塞打开FIFO，那么称该进程内的写操作为设置了阻塞标志的写操作。
对于设置了阻塞标志的写操作：
	1. 当要写入的数据量不大于PIPE_BUF时，linux将保证写入的原子性。
	如果此时管道空闲缓冲区不足以容纳要写入的字节数，则进入睡眠，直到当缓冲区中能够容纳要写入的字节数时，才开始进行一次性写操作。
	2. 当要写入的数据量大于PIPE_BUF时，linux将不再保证写入的原子性。
	FIFO缓冲区一有空闲区域，写进程就会试图向管道写入数据，写操作在写完所有请求写的数据后返回。
对于没有设置阻塞标志的写操作：
	1. 当要写入的数据量大于PIPE_BUF时，linux将不再保证写入的原子性。在写满所有FIFO空闲缓冲区后，写操作返回。
	2. 当要写入的数据量不大于PIPE_BUF时，linux将保证写入的原子性。如果当前FIFO空闲缓冲区能够容纳请求写入的字节数，写完后成功返回；
	如果当前FIFO空闲缓冲区不能够容纳请求写入的字节数，则返回EAGAIN错误，提醒以后再写；
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>

#define FIFO_SERVER "/tmp/fifoserver"
void main(int argc,char** argv)
//第一个命令行参数是请求从FIFO读出的字节数
{
	char r_buf[4096*2];
	int fd;
	int r_size;
	int ret_size;
	r_size=atoi(argv[1]);
	printf("requred real read bytes %d\n",r_size);
	memset(r_buf,0,sizeof(r_buf));
	fd=open(FIFO_SERVER,O_RDONLY|O_NONBLOCK,0);
	//fd=open(FIFO_SERVER,O_RDONLY,0);
	//在此处可以把读程序编译成两个不同版本：阻塞版本及非阻塞版本
	if(fd==-1)
	{
		printf("open %s for read error\n", FIFO_SERVER);
		exit(0);
	}
	while(1)
	{
		memset(r_buf,0,sizeof(r_buf));
		ret_size=read(fd,r_buf,r_size);
		if(ret_size==-1)
			if(errno==EAGAIN)
				printf("no data avlaible\n");
		printf("real read bytes %d\n",ret_size);
		sleep(1);
	}
	pause();
	unlink(FIFO_SERVER);
}