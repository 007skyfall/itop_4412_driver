#ifndef __MPU_IOCTL_H
#define __MPU_IOCTL_H
#include <asm/ioctl.h>

#define type 'a'

#define READ_ACCEL  _IO(type,0)
#define READ_GYRO   _IO(type,1)

struct mpu_data {
	struct{
		short x;
		short y;
		short z;
	}acc ;
	struct {
		short x;
		short y;
		short z;
	}gyro;
};

#endif

