#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "cmd.h"

int main(int argc, const char *argv[])
{
	int fd = 0;

	if((fd = open("/dev/pwm", O_RDWR)) < 0)
	{
		perror("Failed to open");
		return -1;
	}
	else
	{
		printf("Open led success.\n");
	}

	ioctl(fd, PWM_ON);

	getchar();

	ioctl(fd, PWM_OFF);
	

	close(fd);


	return 0;
}

