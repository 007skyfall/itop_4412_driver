#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc,char **argv)
{
	int fd,cmd=2;
	char *read_gpio = "/dev/read_gpioo_ctl";
	char *cmd0 = "0";
	char *cmd1 = "1";
	printf("argc[0] is %s;argv[1] is %s;",argv[0],argv[1]);
	
	if(strcmp(argv[1],cmd0)==0){
		cmd = 0;
	}
	if(strcmp(argv[1],cmd1)==0){
		cmd = 1;
	}
	
/*O_RDWR读写方式,O_NDELAY非阻塞方式*/	
	if((fd = open(read_gpio,O_RDWR|O_NDELAY))<0){
		printf("APP open %s failed",read_gpio);
	}
	else{
		printf("APP open %s success!\n",read_gpio);
		printf("%d io value is %d\n",cmd,ioctl(fd,cmd,0));
	}
	close(fd);
return 0;

}