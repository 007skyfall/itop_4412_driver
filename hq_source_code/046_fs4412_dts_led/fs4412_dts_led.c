#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");


struct resource *res0;
struct resource *res1;

void __iomem * gpf3con;
void __iomem * gpf3dat;


int demo_probe(struct platform_device * pdevice) // 探测函数，一旦设备与驱动匹配成功，就回调此函数
{
	unsigned int dat = 0;

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

	gpf3con = ioremap(res0->start, 4);
	if(gpf3con == NULL)
	{
		printk("Failed to ioremap.\n");
		return -1;
	}

	gpf3dat = ioremap(res1->start, 4);
	if(gpf3dat == NULL)
	{
		printk("Failed to ioremap.\n");
		return -1;	
	}

	// 设置输出模式
	dat = readl(gpf3con);
	dat = (dat & (~(0xf << 20)))|(0x1 << 20);
	writel(dat, gpf3con);

	dat = readl(gpf3dat);
	dat = dat|(0x1 << 5);
	writel(dat, gpf3dat);


	return 0;
}

int demo_remove(struct platform_device * pdevice)  // 移除函数
{
	unsigned int dat = 0;
	printk("%s,%d\n", __func__, __LINE__);


	dat = readl(gpf3dat);
	dat = dat&(~(0x1 << 5));
	writel(dat, gpf3dat);

	iounmap(gpf3con);
	iounmap(gpf3dat);


	return 0;
}

struct of_device_id  idts[] = {
	{.compatible="farsight,led5"},
	{/*Nothing to be done*/},
};

struct platform_driver pdriver = {
	.probe = demo_probe,
	.remove = demo_remove,
	.driver = {
		.name = "fs4412_dts_led",
		.of_match_table = idts,
	},
};

module_platform_driver(pdriver);

