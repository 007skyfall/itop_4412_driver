#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include "mpu_ioctl.h"
/*
（1）模块三要素
（2）struct i2c_driver的定义 + 填充
（3）init ->注册
			probe：
				
					1.封装i2c消息写函数  需要三个参数:addr  reg  data
					2.封装i2c消息度函数  需要三个参数:addr  reg  
					                     返回一个数据:data
					3.对mpu6050进行相关配置工作

					                     
			因为传感器要提供对应的数据，app要读，底层提供读
			    cdev相关
			    
（4）exit ->注销
*/
	
#define SAMPLE_RATE_DIV         0X19
#define CONFIGURATION           0X1A
#define GYRO_CONFIGURATION      0X1B
#define ACCEL_CONFIGURATION     0X1C
#define POWER_MANAGER_1         0X6B

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

void mpu6050_write_reg(struct i2c_client *client ,char reg ,char data)
{
	int ret = 0;
	char tx[2] = {reg,data};
	struct i2c_msg msgs[] = {
		[0] = {.addr = client->addr , .flags = 0 , .len = sizeof(tx), .buf =tx },
	};
	ret = i2c_transfer(client->adapter,msgs,ARRAY_SIZE(msgs));
	if(ret < 0){
		printk("%s fail...\n",__func__);
	}
}
char mpu6050_read_reg(struct i2c_client *client, char reg)
{
	int ret = 0;
	char tx= reg;
	char rx = 0;
	struct i2c_msg msgs[] = {
			{.addr = client->addr , .flags = 0 , .len = sizeof(tx), .buf =&tx},
			{.addr = client->addr , .flags = 1 , .len = sizeof(rx), .buf =&rx}
	};
	ret = i2c_transfer(client->adapter,msgs,ARRAY_SIZE(msgs));
	if(ret < 0){
		printk("%s fail...\n",__func__);
		return -EINVAL;
	}
	return rx;
}

int major = 0;
struct i2c_client *mpu_client;
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

	struct mpu_data data;
	switch(cmd){
		case READ_ACCEL:
			data.acc.x = mpu6050_read_reg(mpu_client, ACCEL_X_H)<<8;
			data.acc.x|=mpu6050_read_reg(mpu_client, ACCEL_X_L);
			data.acc.y = mpu6050_read_reg(mpu_client, ACCEL_Y_H)<<8;
			data.acc.y|=mpu6050_read_reg(mpu_client, ACCEL_Y_L);
			data.acc.z = mpu6050_read_reg(mpu_client, ACCEL_Z_H)<<8;
			data.acc.z|=mpu6050_read_reg(mpu_client, ACCEL_Z_L);
			break;
		case READ_GYRO:
			data.gyro.x = mpu6050_read_reg(mpu_client, GYRO_X_H)<<8;
			data.gyro.x|=mpu6050_read_reg(mpu_client,  GYRO_X_L);
			data.gyro.y = mpu6050_read_reg(mpu_client, GYRO_Y_H)<<8;
			data.gyro.y|=mpu6050_read_reg(mpu_client,  GYRO_Y_L);
			data.gyro.z = mpu6050_read_reg(mpu_client, GYRO_Z_H )<<8;
			data.gyro.z|=mpu6050_read_reg(mpu_client,  GYRO_Z_L);
			break;
		default:
			printk("%s,%d\n",__func__,__LINE__);
			break;	
	}

	if(copy_to_user((void*)arg,&data,sizeof(data))){
		printk("%s,%dcopy_to_user fail...\n",__func__,__LINE__);
		return -EAGAIN;
	}
	return 0;
}

struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = mpu_open,
	.release = mpu_release,
	.unlocked_ioctl = mpu_ioctl,
};

int mpu_probe(struct i2c_client *client , const struct i2c_device_id *id)
{	
	mpu6050_write_reg(client ,POWER_MANAGER_1 ,0x00);
	mpu6050_write_reg(client ,SAMPLE_RATE_DIV ,0x07);
	mpu6050_write_reg(client ,CONFIGURATION ,0x06);
	mpu6050_write_reg(client ,GYRO_CONFIGURATION ,0x18);
	mpu6050_write_reg(client ,ACCEL_CONFIGURATION ,0x00);

	mpu_client = client;
	major = register_chrdev(major,"mpu6050",&f_ops);
	if(major < 0){
		printk("%s,%dregister_chrdev fail...\n",__func__,__LINE__);
		return -EINVAL;
	}
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
int mpu_remove(struct i2c_client *client)
{
	unregister_chrdev(major,"mpu6050");
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}


const struct of_device_id of_tables[] = {
		{.compatible = "farsight,mpu6050"},
		{/*NULL表结束*/}
};
MODULE_DEVICE_TABLE(of,of_tables);

const struct i2c_device_id id_tables[] = {
		{.name = "mpu6050"},
		{.name = "mpu6000"}

};

struct i2c_driver mpu_driver = {
	.probe = mpu_probe,
	.remove = mpu_remove ,
	.driver = {
			.name = "mpu6050",
			.owner = THIS_MODULE,
			.of_match_table = of_match_ptr(of_tables),
	},
	.id_table = id_tables,
};
static int mpu_init(void)
{
	i2c_add_driver(&mpu_driver);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

static void mpu_exit(void)
{
	i2c_del_driver(&mpu_driver);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(mpu_init);
module_exit(mpu_exit);
MODULE_LICENSE("GPL");










