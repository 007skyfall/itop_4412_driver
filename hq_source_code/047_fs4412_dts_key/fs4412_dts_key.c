#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");

int irqno;

irqreturn_t handler(int irqno, void *dev_id)
{
	printk("key2 ok!\n");

	return IRQ_HANDLED;
}

int demo_probe(struct platform_device * pdevice) // 探测函数，一旦设备与驱动匹配成功，就回调此函数
{
	int ret = 0;

	irqno = platform_get_irq(pdevice, 0);
	if(irqno < 0)
	{
		printk("Failed to platform_get_irq.");
		return -1;
	}
	printk("irqno:%d\n", irqno);

	ret = request_irq(irqno, handler, IRQF_DISABLED|IRQF_TRIGGER_FALLING, "key2", NULL);
	if(ret)
	{
		printk("Failed to request_irq.\n");
		return ret;
	}

	
	return 0;
}

int demo_remove(struct platform_device * pdevice)  // 移除函数
{
	printk("%s,%d\n", __func__, __LINE__);

	free_irq(irqno, NULL);

	return 0;
}

struct of_device_id  idts[] = {
	{.compatible="fs4412,key"},
	{/*Nothing to be done*/},
};

struct platform_driver pdriver = {
	.probe = demo_probe,
	.remove = demo_remove,
	.driver = {
		.name = "fs4412_dts_key",
		.of_match_table = idts,
	},
};

module_platform_driver(pdriver);

