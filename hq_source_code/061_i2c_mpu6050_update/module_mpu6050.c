#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "mpu_cmd.h"

/*3.2 对iic从设备的配置工作*/
#define SAMPLE_RATE_DIV   		0X19
#define CONFIGURATION     		0X1A
#define GYRO_CONFIGURATION      0X1B
#define ACCEL_CONFIGURATION     0X1C
#define POWER_MANAGER_1         0X6B
/*数据寄存器*/
#define ACCEL_X_H      0X3B
#define ACCEL_X_L      0X3C
#define ACCEL_Y_H      0X3D
#define ACCEL_Y_L      0X3E
#define ACCEL_Z_H      0X3F
#define ACCEL_Z_L      0X40
#define TEMP_H        0X41
#define TEMP_L        0X42
#define GYRO_X_H      0X43
#define GYRO_X_L      0X44
#define GYRO_Y_H      0X45
#define GYRO_Y_L      0X46
#define GYRO_Z_H      0X47
#define GYRO_Z_L      0X48

int major = 0 ;
#define NAME "mpu6050"
struct i2c_client *mpu_client;
union mpu6050_data data;
/*3.1 对iic从设备的读写操作函数的封装*/
void  mpu6050_write_reg(struct i2c_client *client,unsigned char reg,unsigned char  data)
{
	int ret;	
	unsigned char tx[2] = {reg ,data};
	struct i2c_msg msgs[] = {
		[0] = { .addr  =client->addr , .flags = 0, .len = ARRAY_SIZE(tx) , .buf =tx }
	};

	ret = i2c_transfer(client->adapter,msgs,ARRAY_SIZE(msgs));
	if(ret < 0){
		printk("i2c_transfer fail...%s,%d\n",__func__,__LINE__);
	}
}
unsigned char mpu6050_read_reg(struct i2c_client *client,unsigned char reg)
{
	int ret;
	unsigned char tx = reg;
	unsigned char rx = 0;
	struct i2c_msg msgs[] = {
		[0] = {.addr = client->addr, .flags = 0 ,  .len = 1, .buf = &tx},
		[1] = {.addr = client->addr, .flags = I2C_M_RD,  .len = 1, .buf = &rx},
	};
	ret = i2c_transfer(client->adapter,msgs,ARRAY_SIZE(msgs));
	if(ret < 0){
		printk("i2c_transfer fail...%s,%d\n",__func__,__LINE__);
	}
	return rx;
}

int mpu_open(struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
int mpu_release(struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

long mpu_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch(cmd){
		case ACCEL:
			data.acc.x  = mpu6050_read_reg(mpu_client,ACCEL_X_H)<<8;
			data.acc.x |= mpu6050_read_reg(mpu_client,ACCEL_X_L);
			data.acc.y  = mpu6050_read_reg(mpu_client,ACCEL_Y_H)<<8;
			data.acc.y |= mpu6050_read_reg(mpu_client,ACCEL_Y_L);
			data.acc.z  = mpu6050_read_reg(mpu_client,ACCEL_Z_H)<<8;
			data.acc.z |= mpu6050_read_reg(mpu_client,ACCEL_Z_L);
			break;
		case GYRO:
			data.gyro.x  = mpu6050_read_reg(mpu_client,GYRO_X_H)<<8;
			data.gyro.x |= mpu6050_read_reg(mpu_client,GYRO_X_L);
			data.gyro.y  = mpu6050_read_reg(mpu_client,GYRO_Y_H)<<8;
			data.gyro.y |= mpu6050_read_reg(mpu_client,GYRO_Y_L);
			data.gyro.z  = mpu6050_read_reg(mpu_client,GYRO_Z_H)<<8;
			data.gyro.z |= mpu6050_read_reg(mpu_client,GYRO_Z_L);
			break;
		case TEMP:
			data.temp  = mpu6050_read_reg(mpu_client,TEMP_H)<<8;
			data.temp |= mpu6050_read_reg(mpu_client,TEMP_L);
			break;
		default:
			printk("default ....%s,%d\n",__func__,__LINE__);
			break;
	}
	if(copy_to_user((void*)arg , &data ,sizeof(data))){
		printk("copy_to_user ...%s,%d\n",__func__,__LINE__);
	}
	//printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = mpu_open,
	.release = mpu_release ,
	.unlocked_ioctl = mpu_ioctl,
};
/*
	1.模块三要素
	2.i2c_driver的定义，填充，注册
	3.iic的硬件相关
		3.1 对iic从设备的读写操作函数的封装
		3.2 对iic从设备的配置工作
		3.3 因为想通过应用层获取硬件的加速度  陀螺  温度数据，故应提供对应的设备文件
		   【字符设备相关的】
*/


int mpu_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	mpu6050_write_reg(client,POWER_MANAGER_1,0X00);
	mpu6050_write_reg(client,SAMPLE_RATE_DIV,0X07);
	mpu6050_write_reg(client,CONFIGURATION,0X00);
	mpu6050_write_reg(client,GYRO_CONFIGURATION,0X18);/*-2000   +2000*/
	mpu6050_write_reg(client,ACCEL_CONFIGURATION,0X18);/* -16g 16g*/
	mpu_client = client;
	/*3.3 因为想通过应用层获取硬件的加速度  陀螺  温度数据，故应提供对应的设备文件*/
	major = register_chrdev(major,NAME,&f_ops);
	if(major < 0){
		printk("register_chrdev ...%s,%d\n",__func__,__LINE__);
		return -EINVAL;
	}
	printk("major:%d\t%s,%d\n",major,__func__,__LINE__);
	return 0;
}
int mpu_remove(struct i2c_client *client)
{
    unregister_chrdev(major,NAME);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

struct i2c_device_id id_tables[] = {
	[0] = {},
};

struct of_device_id	of_table[] = {
	[0] = {.compatible = "fs4412,mpu6050"},
	[1] = {/*NULL表征结束*/}
};
MODULE_DEVICE_TABLE(of,of_table);

struct i2c_driver mpu_drv = {
	.probe = mpu_probe,
	.remove = mpu_remove,
	.driver = {
		.name = "mpuuuuuuu",
		.owner = THIS_MODULE,
	    .of_match_table = of_match_ptr(of_table),
	},
	.id_table = id_tables,
};

static int __init mpu_init(void)
{
	i2c_add_driver(&mpu_drv);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

static void __exit mpu_exit(void)
{
	i2c_del_driver(&mpu_drv);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(mpu_init);
module_exit(mpu_exit);
MODULE_LICENSE("GPL");


