#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc,char *argv[])
{
	int fd,ret;
	char *i2c_device = "/dev/i2c_control";
	unsigned char buffer[2];
	
	printf("open %s!\n",i2c_device);
	if((fd = open(i2c_device,O_RDWR|O_NDELAY))<0)
		printf("APP open %s failed",i2c_device);
	else{
		printf("APP open %s success!\n",i2c_device);
	}
	
//读一个数据0xa6
	buffer[0] = 0xa6;
	ret = read(fd,buffer,1);
	if(ret<0)
		printf("i2c read failed!\n");
	else{
		printf("i2c read reg 0xa6 data is 0x%02x!\n",buffer[0]);
	}
	
//01先从0x00读出一个数据，02写一个数据到0x00，03再读出来对比
	//01
	buffer[0] = 0x00;
	read(fd,buffer,1);
	printf("i2c read reg 0x00 data is 0x%02x!\n",buffer[0]);
	//02
	buffer[0] = 0x00;
	buffer[1] = 0x40;
	ret = write(fd,buffer,2);
	if(ret<0){
		printf("i2c write failed!\n");
		goto exit;
	}
	//03
	buffer[0] = 0x00;
	read(fd,buffer,1);
	printf("i2c read reg 0x00 data is 0x%02x!\n",buffer[0]);
	
	close(fd);


exit:
	close(fd);
	return -1;
	
return 0;
}