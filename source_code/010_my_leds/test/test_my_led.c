#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char * agrv[])
{
	int fd;
	char *hello_node = "/dev/hello_ctl";
	
/*O_RDWR读写方式,O_NDELAY非阻塞方式*/	
	if((fd = open(hello_node,O_RDWR|O_NDELAY))<0)
	{
		printf("APP open %s failed\n",hello_node);
	}
	else
	{
		printf("APP open %s success\n",hello_node);
		ioctl(fd,1,1);
		sleep(3);
		ioctl(fd,0,1);
		sleep(3);
		ioctl(fd,1,1);
	}
	
	close(fd);
return 0;

}