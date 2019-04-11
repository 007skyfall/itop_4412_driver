#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/fs.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
//#include <mach/gpio-bank.h>
#include <mach/regs-gpio.h>
#include <asm/io.h>
#include <linux/regulator/consumer.h>
//#include "gps.h"
#include <linux/delay.h>

//中断头文件
#include <linux/irq.h>
#include <linux/interrupt.h>

#define DPRINTK(x...) printk("keyirq DEBUG:" x)

#define DRIVER_NAME "keyirq"

static irqreturn_t eint9_interrupt(int irq,void *dev_id)
{
	printk("receive a interrupt 9!\n");
	return IRQ_HANDLED;
}

static irqreturn_t eint10_interrupt(int irq,void *dev_id)
{
	printk("receive a interrupt 10!\n");
	return IRQ_HANDLED;
}

static int keyirq_probe(struct platform_device *pdev)
{
	//int ret, i;
	char *banner = "keyirq Initialize\n";
	printk(banner);
	//注册中断
	request_irq(IRQ_EINT(9),eint9_interrupt,IRQ_TYPE_EDGE_FALLING,"my_eint9",pdev);
	request_irq(IRQ_EINT(10),eint10_interrupt,IRQ_TYPE_EDGE_FALLING,"my_eint10",pdev);
	
	return 0;
}

static int keyirq_remove (struct platform_device *pdev)
{
	free_irq(IRQ_EINT(9),pdev);
	free_irq(IRQ_EINT(10),pdev);
	
	return 0;
}

static int keyirq_suspend (struct platform_device *pdev, pm_message_t state)
{
	DPRINTK("keyirq suspend:power off!\n");
	return 0;
}

static int keyirq_resume (struct platform_device *pdev)
{
	DPRINTK("keyirq resume:power on!\n");
	return 0;
}

static struct platform_driver keyirq_driver = {
	.probe = keyirq_probe,
	.remove = keyirq_remove,
	.suspend = keyirq_suspend,
	.resume = keyirq_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	},
};

static void __exit keyirq_exit(void)
{
	platform_driver_unregister(&keyirq_driver);
}

static int __init keyirq_init(void)
{
	return platform_driver_register(&keyirq_driver);
}

module_init(keyirq_init);
module_exit(keyirq_exit);

MODULE_LICENSE("Dual BSD/GPL");
