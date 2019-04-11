#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/platform_device.h>


MODULE_LICENSE("GPL");

struct resource res[2] = {
	[0] = {
		.start = 0x10003000,
		.end = 0x10003003,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = 0x20004000,
		.end = 0x20008000,
		.flags = IORESOURCE_MEM,
	},
};

void demo_release(struct device *dev)
{
	printk("%s,%d\n", __func__, __LINE__);
}

struct platform_device  pdevice = {
	.name = "platform_original",
	.num_resources = ARRAY_SIZE(res),
	.resource = res,
	.dev = {
		.release = demo_release,
	},
};

static int __init demo_init(void)
{
	printk("%s,%d\n", __func__, __LINE__);

	platform_device_register(&pdevice);

	return 0;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n", __func__, __LINE__);
	platform_device_unregister(&pdevice);
}

module_init(demo_init);
module_exit(demo_exit)
	
