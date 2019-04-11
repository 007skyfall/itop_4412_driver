#include <linux/init.h>
#include <linux/module.h>

/*驱动注册的头文件，包含驱动的结构体和注册和卸载的函数*/
#include <linux/platform_device.h>

#define DRIVER_NAME "hello_ctl"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("SKYFALL");

static int hello_probe(struct platform_device *pdv){
	
	printk(KERN_EMERG "\tinitialized\n");
	
	printk("%s,%d\n",__func__,__LINE__);
	
	return 0;
}

static int hello_remove(struct platform_device *pdv){
	
	return 0;
}

static void hello_shutdown(struct platform_device *pdv){
	
	;
}

static int hello_suspend(struct platform_device *pdv){
	
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
	printk("%s,%d\n",__func__,__LINE__);
	platform_driver_unregister(&hello_driver);	
}

module_init(hello_init);
module_exit(hello_exit);

