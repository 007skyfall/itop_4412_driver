#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char *argv[])
{
	int fd;
	char *hello_node = "/dev/hello_ctl123";
//	char *hello_node = "./test.txt";	
/*O_RDWR读写方式打开,O_NDELAY非阻塞方式*/	
	if((fd = open(hello_node,O_RDWR|O_NDELAY))<0){
		printf("APP open %s failed\n",hello_node);
	}
	else{
		printf("APP open %s success\n",hello_node);
		ioctl(fd,1,6);
	}
	
	close(fd);
	return 0;
}

