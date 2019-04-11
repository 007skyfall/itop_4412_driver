#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ioctl.h>
#include "../cmd.h"

#define  N  128

int main(int argc, const char *argv[])
{
	int fd;
//	char buf[N] = "This is a ioctl test.";
//	int ret = 0;

	fd = open("/dev/hello", O_RDWR);
	if(fd < 0)
	{
		printf("Failed to open.\n");
		return -1;
	}
	else
	{
		printf("open success.\n");
	}

	getchar();

	ioctl(fd, CMD);
	printf("after cmd.\n");

	ioctl(fd, CMDW, 5);	
	printf("cmdw ok!\n");

	

	close(fd);


	return 0;
}
