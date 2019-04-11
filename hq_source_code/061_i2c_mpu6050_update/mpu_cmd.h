#ifndef __MPU_CMD_H
#define __MPU_CMD_H


#include <asm/ioctl.h>

#define TYPE  'A'
#define ACCEL _IO(TYPE,0)
#define GYRO  _IO(TYPE,1)
#define TEMP  _IO(TYPE,2)

union mpu6050_data{
    unsigned short temp;
	struct {
		unsigned short x;
		unsigned short y;
		unsigned short z;
	}acc;
	struct {
		unsigned short x;
		unsigned short y;
		unsigned short z;
	}gyro;

};




#endif
