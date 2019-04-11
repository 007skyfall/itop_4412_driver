#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

/*argv[1] is cmd , argv[2] is io_arg*/
int main(int argc , char **argv){
	int fd;
	char *lednode = "/dev/chardevnode0";

	/*O_RDWR读写方式,O_NDELAY非阻塞方式*/	
	if((fd = open(lednode,O_RDWR|O_NDELAY))<0){
		printf("APP open %s failed!\n",lednode);
	}
	else{
		printf("APP open %s success!\n",lednode);
			ioctl(fd,atoi(argv[1]),atoi(argv[2]));
			printf("APP ioctl %s ,cmd is %s! io_arg is %s!\n",lednode,argv[1],argv[2]);
	}
	
	close(fd);
return 0;

}