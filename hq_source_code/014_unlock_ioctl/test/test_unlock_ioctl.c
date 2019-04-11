#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "myioctl.h"
 #include <sys/ioctl.h>

#define PATH "/dev/demo_ioctl"

int main(int argc, const char *argv[])
{
	int fd ;
	fd = open(PATH,O_RDWR);
	if(fd < 0)
	{
		perror("open");
	}
	if(ioctl(fd,ONE))
	{
		perror("ioctl");
	}
	if(ioctl(fd,TWO))
	{
		perror("ioctl");
	}
	close(fd);
	return 0;
}
