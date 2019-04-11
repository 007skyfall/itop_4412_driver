#include <linux/init.h>
#include <linux/module.h>

/*驱动注册的头文件，包含驱动的结构体和注册和卸载的函数*/
#include <linux/platform_device.h>
/*注册杂项设备头文件*/
#include <linux/miscdevice.h>
/*注册设备节点的文件结构体*/
#include <linux/fs.h>

/*Linux中申请GPIO的头文件*/
#include <linux/gpio.h>
/*三星平台的GPIO配置函数头文件*/
/*三星平台EXYNOS系列平台，GPIO配置参数宏定义头文件*/
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
/*三星平台4412平台，GPIO宏定义头文件*/
#include <mach/gpio-exynos4.h>

#define DRIVER_NAME "read_gpio_ctl"
#define DEVICE_NAME "read_gpioo_ctl"


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("SKYFALL");

//应用里面通过ioctl返回值来获取管脚电平
static long read_gpio_ioctl( struct file *files, unsigned int cmd, unsigned long arg){
	printk("cmd is %d,arg is %d\n",cmd,arg);
	
	//参数cmd可以是0或者1
	if(cmd > 1){
		printk(KERN_EMERG "cmd is 0 or 1\n");
	}
	if(arg > 1){
		printk(KERN_EMERG "arg is only 1\n");
	}
	
	//如果cmd是0，则返回EXYNOS4_GPC0(3)，拨码开关3对应管脚的输入电平
	//如果cmd是1，则返回EXYNOS4_GPX0(6)，拨码开关4对应管脚的输入电平
	if(cmd == 0){
		return gpio_get_value(EXYNOS4_GPC0(3));
	}
	if(cmd == 1){
		return gpio_get_value(EXYNOS4_GPX0(6));
	}
	return 0;
}

static int read_gpio_release(struct inode *inode, struct file *file){
	printk("%s,%d\n",__func__,__LINE__);
	printk(KERN_EMERG "hello release\n");
	return 0;
}

static int read_gpio_open(struct inode *inode, struct file *file){
	printk("%s,%d\n",__func__,__LINE__);
	printk(KERN_EMERG "hello open\n");
	return 0;
}

static struct file_operations read_gpio_ops = {
	.owner = THIS_MODULE,
	.open = read_gpio_open,
	.release = read_gpio_release,
	.unlocked_ioctl = read_gpio_ioctl,
};

static  struct miscdevice read_gpio_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &read_gpio_ops,
};


static int read_gpio_probe(struct platform_device *pdv){
	int ret;
	
	printk(KERN_EMERG "\tinitialized\n");
	
	ret = gpio_request(EXYNOS4_GPC0(3),"SWITCH3");
	if(ret < 0){
		printk(KERN_EMERG "gpio_request EXYNOS4_GPC0(3)failed!\n");
		return ret;
	}
	else{
		//设置为输入模式,不进行上拉下拉
		s3c_gpio_cfgpin(EXYNOS4_GPC0(3),S3C_GPIO_INPUT); 
		s3c_gpio_setpull(EXYNOS4_GPC0(3),S3C_GPIO_PULL_NONE);		
	}
	
	ret = gpio_request(EXYNOS4_GPX0(6),"SWITCH4");
	if(ret < 0){
		printk(KERN_EMERG "gpio_request EXYNOS4_GPX0(6) failed!\n");
		return ret;
	}
	else{
		s3c_gpio_cfgpin(EXYNOS4_GPX0(6),S3C_GPIO_INPUT); 
		s3c_gpio_setpull(EXYNOS4_GPX0(6),S3C_GPIO_PULL_NONE);
	}
	
	misc_register(&read_gpio_dev);
	
	return 0;
}

static int read_gpio_remove(struct platform_device *pdv){
	
	printk(KERN_EMERG "\tremove\n");
	misc_deregister(&read_gpio_dev);
	return 0;
}

static void read_gpio_shutdown(struct platform_device *pdv){
	
	;
}

static int read_gpio_suspend(struct platform_device *pdv,pm_message_t pmt){
	
	return 0;
}

static int read_gpio_resume(struct platform_device *pdv){
	
	return 0;
}

struct platform_driver read_gpio_driver = {
	.probe = read_gpio_probe,
	.remove = read_gpio_remove,
	.shutdown = read_gpio_shutdown,
	.suspend = read_gpio_suspend,
	.resume = read_gpio_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};


static int read_gpio_init(void)
{
	int DriverState;
	printk("%s,%d\n",__func__,__LINE__);
	printk(KERN_EMERG "HELLO WORLD enter!\n");
	DriverState = platform_driver_register(&read_gpio_driver);
	
	printk(KERN_EMERG "\tDriverState is %d\n",DriverState);
	
	return 0;
}


static void read_gpio_exit(void)
{
	printk(KERN_EMERG "HELLO WORLD exit!\n");
	
	platform_driver_unregister(&read_gpio_driver);	
}

module_init(read_gpio_init);
module_exit(read_gpio_exit);
