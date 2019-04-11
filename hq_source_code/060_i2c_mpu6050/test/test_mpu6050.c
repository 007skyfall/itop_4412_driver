#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "mpu_ioctl.h"

struct  mpu_data data;
int main(int argc, const char *argv[])
{
	int fd ;
	fd = open("/dev/mpu6050",O_RDWR);
	if(fd < 0){
		perror("open");
	}

	while(1){
		if(ioctl(fd,READ_ACCEL,&data)){
			perror("ioctl");
		}
		printf("accx:%d\taccy:%d\taccz:%d\n",data.acc.x,data.acc.y,data.acc.z);
	}
	close(fd);
	return 0;
}


