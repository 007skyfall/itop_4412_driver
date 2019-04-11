#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include "cmd.h"


#define  N  128

int main(int argc, const char *argv[])
{
	int fd;
	struct node n1;
	struct node n2;

	fd = open("/dev/test_module_ioctl", O_RDWR);
	if(fd < 0)
	{
		perror("Failed to open.");
		return -1;
	}
	else
	{
		printf("open success.\n");
	}

	getchar();

	ioctl(fd, CMD);
	printf("CMD done.\n");

	getchar();
	n1.val = 100;
	strcpy(n1.buf, "This is a cmdw test.");

	ioctl(fd, CMDW, &n1);
	printf("cmdw done.\n");

	getchar();
	ioctl(fd, CMDR, &n2);
	printf("cmdr done.\n");
	printf("val:%d , %s\n", n2.val, n2.buf);

	close(fd);

	return 0;
}
