#include <linux/init.h>
#include <linux/module.h>

/*驱动注册的头文件，包含驱动的结构体和注册和卸载的函数*/
#include <linux/platform_device.h>
/*注册杂项设备头文件*/
#include <linux/miscdevice.h>
/*注册设备节点的文件结构体*/
#include <linux/fs.h>

#define DRIVER_NAME "hello_ctl"
#define DEVICE_NAME "hello_ctl123"
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("SKYFALL");

//long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
//驱动实际操作的实现
static long hello_ioctl( struct file *file, unsigned int cmd, unsigned long arg){

	printk("%s,%d\n",__func__,__LINE__);
	printk("cmd is %d,arg is %d\n",cmd,arg);
	return 0;
}

//int (*release) (struct inode *, struct file *);
//关闭该驱动的实现
static int hello_release(struct inode *inode, struct file *file){
	printk("%s,%d\n",__func__,__LINE__);
	printk(KERN_EMERG "hello release\n");
	return 0;
}

//int (*open) (struct inode *, struct file *);
//打开该驱动的实现
static int hello_open(struct inode *inode, struct file *file){
	printk("%s,%d\n",__func__,__LINE__);
	printk(KERN_EMERG "hello open\n");
	return 0;
}

//定义一个字符设备操作集合
static struct file_operations hello_ops = {
	.owner = THIS_MODULE,
	.open = hello_open,
	.release = hello_release,
	.unlocked_ioctl = hello_ioctl,
};

//定义一个杂项设备结构体
static  struct miscdevice hello_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	//字符设备操作集合，需要提前定义一个，然后取地址
	.fops = &hello_ops,
};


static int hello_probe(struct platform_device *pdv){
	
	printk(KERN_EMERG "\tinitialized\n");
	printk("%s,%d\n",__func__,__LINE__);
	
	//注册杂项设备，需要一个传一个杂项设备的结构体，所以需要提前定义一个，然后取地址
	misc_register(&hello_dev);
	
	return 0;
}

static int hello_remove(struct platform_device *pdv){
	
	printk(KERN_EMERG "\tremove\n");
	//注销杂项设备
	misc_deregister(&hello_dev);
	return 0;
}

static void hello_shutdown(struct platform_device *pdv){
	
	;
}

static int hello_suspend(struct platform_device *pdv,pm_message_t pmt){
	
	return 0;
}

static int hello_resume(struct platform_device *pdv){
	
	return 0;
}

struct platform_driver hello_driver = {
	.probe = hello_probe,
	.remove = hello_remove,
	.shutdown = hello_shutdown,
	.suspend = hello_suspend,
	.resume = hello_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};


static int hello_init(void)
{
	int DriverState;//定义驱动状态，判断驱动是否注册成功
	
	printk(KERN_EMERG "HELLO WORLD enter!\n");
	printk("%s,%d\n",__func__,__LINE__);
	DriverState = platform_driver_register(&hello_driver);
	/*
	驱动一旦注册成功，会与设备进行匹配，匹配成功是由
	platform_match函数进行匹配，驱动调用probe初始化函数
	*/
	
	printk(KERN_EMERG "\tDriverState is %d\n",DriverState);
	return 0;
}


static void hello_exit(void)
{
	printk(KERN_EMERG "HELLO WORLD exit!\n");
	
	platform_driver_unregister(&hello_driver);	
}

module_init(hello_init);
module_exit(hello_exit);
