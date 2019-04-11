#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

int main(int argc,char *argv[])
{

	char *atomic_bit = "/dev/atomic_bit";
	int fd1;
	
	if((fd1 = open(atomic_bit,O_RDWR|O_NDELAY))==0)
	{
		printf("%s open %s success!\n",argv[0],atomic_bit);
	}
	else
	{	
		perror(argv[0]);
	}	
	
	while(1);
	close(fd1);
	
retun 0;
	
}
