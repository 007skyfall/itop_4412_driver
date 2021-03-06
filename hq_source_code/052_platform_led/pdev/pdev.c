#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
/*
流程（platform_device端一般用于提供设备信息）：		
（1）模块三要素				
（2）定义并填充对应的platform_device结构体	
（3）注册、注销即可
*/
void pdev_release(struct device *dev)
{
	printk("%s,%d\n",__func__,__LINE__);
}

struct resource res[] = {
	[0] = {//con
		.flags = IORESOURCE_MEM,
		.start = 0x114001e0,
		.end = 0X114001E3,
	},
	[1] = {//dat
		.flags = IORESOURCE_MEM,
		.start = 0x114001e4,
		.end = 0x114001e7,
	}
};

struct platform_device pdev = {
	.name = "pdev",
	.id = -1,
	.dev = {
		.release = pdev_release,
	},
	.num_resources = ARRAY_SIZE(res),//计算数组成员个数
	.resource = res,
};

static int pdev_init(void)
{
	platform_device_register(&pdev);//  /sys/bus/platform/devices/pdev
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

static void pdev_exit(void)
{
	platform_device_unregister(&pdev);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(pdev_init);
module_exit(pdev_exit);
MODULE_LICENSE("GPL");

