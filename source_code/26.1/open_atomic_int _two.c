#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc,char *argv[])
{
	char *atomic_int = "/dev/atomic_int";
	int fd;
	
	if((fd = open(atomic_int,O_RDWR|O_NDELAY))<0)
	{
		printf("%s open %s failed!\n",argv[0],atomic_int);
	}
	else
	{
		printf("%s open %s sucess!\n",argv[0],atomic_int);
	}
	
	while(1);
	
return 0;
}