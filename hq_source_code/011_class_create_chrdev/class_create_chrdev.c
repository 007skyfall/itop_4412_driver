#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>//printk
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
/*
1.模块三要素			
2.完成字符设备框架相关的代码cdev
	2.1 设备号申请
	2.2 cdev的申请
	2.3 cdev与struct file_operations 的绑定
	2.4 cdev的注册
*/
#define CLASS_NAME "demo_class"
#define NAME "demo"
dev_t from;
int baseminor = 0;
int count = 3;
struct class *my_class = NULL;
struct device *my_device = NULL;
struct cdev *my_cdev = NULL;

int demo_open(struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
int demo_release(struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.release = demo_release,
};
int major = 0;
int minor = 0;

int demo_init(void)
{
	int ret = -1;
	int i = 0;
	
	ret = alloc_chrdev_region(&from,baseminor,count,NAME);
	if(ret){
		printk("%s,%d alloc_chrdev_region fail\n",__func__,__LINE__);
		return -EINVAL;
	}
	
	major = MAJOR(from);
	minor = MINOR(from);
	
	my_cdev = cdev_alloc();
	if(my_cdev == NULL){
		printk("%s,%d cdev_alloc fail\n",__func__,__LINE__);
		goto ERR_STEP1;
	}

	cdev_init(my_cdev,&f_ops);

	cdev_add(my_cdev,from,count);
	/**********************************************/
	my_class = class_create(THIS_MODULE,CLASS_NAME);//  /sys/class/CLASS_NAME
	if(IS_ERR(my_class)){
		printk("%s,%d class_create fail...\n",__func__,__LINE__);	
		ret = PTR_ERR(my_class);
		goto ERR_STEP2;
	}

	for(i = 0 ; i < count ; i++){// 250 0; 250 1 ;250 2; 
		my_device = device_create(my_class,NULL,MKDEV(major,i),NULL,"%s%d",NAME,i);
		if(IS_ERR(my_device)){
			printk("%s,%d device_create fail...\n",__func__,__LINE__);	
			ret = PTR_ERR(my_device);
			goto ERR_STEP2;
		}
	}
	printk("%s,%d\n",__func__,__LINE__);
	return 0;

ERR_STEP2:
	for(i-- ; i >= 0 ;i--)
		device_destroy(my_class,MKDEV(major,i));
	class_destroy(my_class);
	cdev_del(my_cdev);
ERR_STEP1:
	unregister_chrdev_region(from,count);
	return ret;
	
}

void demo_exit(void)
{
	int i;
	for(i= 0  ; i < count; i++)
	device_destroy(my_class,MKDEV(major,i));
	class_destroy(my_class);
	cdev_del(my_cdev);
	unregister_chrdev_region(from,count);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");

