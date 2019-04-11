#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <signal.h>

#define  N  128

int fd;
char buf[N] = {};

void handler(int sig)
{
	read(fd, buf, N);
	printf("buf:%s\n", buf);
}

int main(int argc, const char *argv[])
{
	int flags = 0;

	fd = open("/dev/module_fasync_io", O_RDWR);
	if(fd < 0)
	{
		perror("Failed to open.");
		return -1;
	}
	else
	{
		printf("open success.\n");
	}

//	getchar();

	flags = fcntl(fd, F_GETFL);
	flags |= O_ASYNC;
	fcntl(fd, F_SETFL, flags);

//	getchar();

	fcntl(fd, F_SETOWN, getpid());

//	getchar();

	signal(SIGIO, handler);

//	getchar();

	while(1)
	{
		printf("++++++++++++++++++++=\n");
		sleep(1);
	}


	close(fd);

	return 0;
}
