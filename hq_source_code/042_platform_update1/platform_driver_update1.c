#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/platform_device.h>


MODULE_LICENSE("GPL");

struct resource *res0;
struct resource *res1;

int demo_probe(struct platform_device * pdevice) // 探测函数，一旦设备与驱动匹配成功，就回调此函数
{
	printk("probe ok!\n");

	res0 = platform_get_resource(pdevice, IORESOURCE_MEM, 0);
	if(res0 == NULL)
	{
		printk("Failed to platform_get_resource.\n");
		return -1;
	}
	printk("res0:%#x , %#x\n", (unsigned int)res0->start, (unsigned int)res0->end);

	res1 = platform_get_resource(pdevice, IORESOURCE_MEM, 1);
	if(res1 == NULL)
	{
		printk("Failed to platform_get_resource.\n");
		return -1;
	}
	printk("res1:%#x , %#x\n", (unsigned int)res1->start, (unsigned int)res1->end);


	return 0;
}

int demo_remove(struct platform_device * pdevice)  // 移除函数
{
	printk("%s,%d\n", __func__, __LINE__);

	return 0;
}

struct platform_device_id  ids[] = {
	{.name = "demo0", },
	{.name = "demo1", },
	{/*Nothing to be done.*/},
};

struct platform_driver pdriver = {
	.probe = demo_probe,
	.remove = demo_remove,
	.driver = {
		.name = "platform_driver_update1",
	},
	.id_table = ids, 
};

module_platform_driver(pdriver);

