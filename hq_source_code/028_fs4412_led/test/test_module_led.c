#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "cmd.h"

int main(int argc, const char *argv[])
{
	int fd = 0;
	int i = 1;

	if((fd = open("/dev/test_fs4412_led", O_RDWR)) < 0)
	{
		perror("Failed to open");
		return -1;
	}
	else
	{
		printf("Open led success.\n");
	}

	while(1)
	{
		ioctl(fd, LED_ON, &i);
		sleep(1);
		ioctl(fd, LED_OFF, &i);
		
		i++;
		if(i == 5)
			i = 1;
	}

	close(fd);


	return 0;
}

