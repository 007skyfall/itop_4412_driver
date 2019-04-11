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
	//int ret = 0;
	struct node c1;
	struct node c2;

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

	c1.val = 200;
	strcpy(c1.buf, "This is a ioctl test.");
	ioctl(fd, CMDW, &c1);	
	printf("cmdw ok!\n");

	getchar();
	
	ioctl(fd, CMDR, &c2);
	printf("%d,%s\n", c2.val, c2.buf);

	close(fd);


	return 0;
}
