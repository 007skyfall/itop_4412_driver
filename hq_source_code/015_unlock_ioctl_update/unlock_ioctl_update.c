#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/device.h>

#include "myioctl.h"

#define NAME "demo"
#define CLASS_NAME "demo_class"

#define MAX  64
int count = 3;
char ker_buf[MAX] = {0};
/*
1.模块三要素
2.字符框架相关
	2.1 申请设备号
	2.2 cdev申请
	2.3 cdev初始化
	2.4 cdev注册
*/
ssize_t demo_read(struct file *file, char __user *user, size_t size, loff_t *loff)
{
	if(size > MAX)   size = MAX;
	
	if(raw_copy_to_user(user,ker_buf,sizeof(ker_buf))){
		printk("copy_to_user fail...%s,%d\n",__func__,__LINE__);
		return -EAGAIN;
	}
	
	printk("%s,%d\n",__func__,__LINE__);
	return size;
}

ssize_t demo_write(struct file *file, const char __user *user ,size_t size, loff_t *loff)
{
	if(size > MAX) size = MAX;

	if(raw_copy_from_user(ker_buf,user,MAX)){
		printk("copy_from_user fail...%s,%d\n",__func__,__LINE__);
		return -EAGAIN;
	}
	printk("%s,%d\n",__func__,__LINE__);
	return size;
}

long demo_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch(cmd){
		case ONE:
			printk("111111111111111111111111\n");
			break;
		case TWO:
			printk("2222222222222222222222221\n");
			break;
		default:
			printk("%s,%d\n",__func__,__LINE__);
			break;
	}
	printk("%s,%d\n",__func__,__LINE__);
	return 0;	
}

int demo_open (struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
int demo_close (struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

int major = 0 , minor = 0;

struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.release = demo_close,
	.read = demo_read, 
	.write = demo_write,
	.unlocked_ioctl = demo_ioctl,
};
struct class *cls = NULL;
struct device *dev = NULL;
int __init demo_init(void)
{
	int ret;
	int i;
	major = register_chrdev(major,NAME,&f_ops);
	if(major < 0){
		printk("register_chrdev fail ...%s,%d\n",__func__,__LINE__);
		return -EINVAL;
	}

	/*自动创建设备节点*/
	cls = class_create(THIS_MODULE,CLASS_NAME);
	if(IS_ERR(cls)){
		ret = PTR_ERR(cls);
		goto ERR_STEP1;
	}
	//device :250 0
	//device :250 1
	//dev_no = MKDEV(major , minor );
	for(i = minor ; i  < minor + count ; i++){
		dev = device_create(cls,NULL,MKDEV(major,i),NULL,"%s%d",NAME,i);
		if(IS_ERR(dev)){
			ret = PTR_ERR(dev);
			goto ERR_STEP2;
		}
	}
	printk("major :%d \t %s,%d\n",major ,__func__,__LINE__);
	return 0;
	
ERR_STEP2:	
	for(i-- ; i >= minor ; i--)
		device_destroy(cls,MKDEV(major,i));
	class_destroy(cls);
ERR_STEP1:
	unregister_chrdev(major,NAME);
	return ret;

}

void __exit demo_exit(void)
{
	int i;
	for(i = 0 ; i < minor + count  ; i++)
		device_destroy(cls,MKDEV(major,i));
	class_destroy(cls);
	unregister_chrdev(major,NAME);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
