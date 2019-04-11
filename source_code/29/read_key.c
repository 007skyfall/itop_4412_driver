#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char *argv[])
{
	int fd;
	unsigned char buffer[2];
	char *read_key = "/dev/pollkey";

/*O_RDWR��д��,O_NDELAY��������ʽ*/	
	if((fd = open(read_key,O_RDWR|O_NDELAY))<0){
		printf("APP open %s failed\n",read_key);
		return -1;
	}
	printf("APP open %s success!\n",read_key);
	while(1){
		read(fd,buffer,sizeof(buffer));
		if(!buffer[0] || !buffer[1]){
			printf("key home value is %d\n",buffer[0]);
			printf("key back value is %d\n",buffer[1]);
		}
	}
	
	close(fd);
	
return 0;
}