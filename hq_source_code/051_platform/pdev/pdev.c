#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
/*
���̣�platform_device��һ�������ṩ�豸��Ϣ����		
��1��ģ����Ҫ��				
��2�����岢����Ӧ��platform_device�ṹ��	
��3��ע�ᡢע������
*/
void pdev_release(struct device *dev)
{
	printk("%s,%d\n",__func__,__LINE__);
}

struct resource res[] = {
	[0] = {
		.flags = IORESOURCE_MEM,
		.start = 0X114001E0,
		.end = 0X114001E3,
	},
	[1] = {
		.flags = IORESOURCE_IRQ,
		.start = 57,
		.end = 57,
	}
};

struct platform_device pdev = {
	.name = "pdev",
	.id = -1,
	.dev = {
		.release = pdev_release,
	},
	.num_resources = ARRAY_SIZE(res),//���������Ա����
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





