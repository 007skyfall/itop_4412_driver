#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <linux/cdev.h>


MODULE_LICENSE("GPL");

dev_t devno;
int minor = 0;
int count = 1;

struct cdev *pdev;
struct class *pclass;
struct device *devicep;

void __iomem * gpf3con;
void __iomem * gpf3dat;

struct resource *res0;
struct resource *res1;

int demo_open(struct inode *inodep, struct file *filep)
{
	unsigned int dat = 0;
	printk("%s,%d\n", __func__, __LINE__);
	
	dat = readl(gpf3dat);
	dat = dat|(0x1 << 5);
	writel(dat, gpf3dat);

	return 0;
}

int demo_release(struct inode *inodep, struct file * filep)
{
	unsigned int dat = 0;

	printk("%s,%d\n", __func__, __LINE__);

	dat = readl(gpf3dat);
	dat = dat&(~(0x1 << 5));
	writel(dat, gpf3dat);

	return 0;
}


struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.release = demo_release,
};


int demo_probe(struct platform_device * pdevice) // 探测函数，一旦设备与驱动匹配成功，就回调此函数
{
	int ret = 0;
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

	ret = alloc_chrdev_region(&devno, minor, count, "platform_led");
	if(ret)
	{
		printk("Failed to alloc_chrdev_region.\n");
		return ret;
	}
	printk("devno:%d , major:%d  minor:%d\n", devno, MAJOR(devno), MINOR(devno));

	pdev = cdev_alloc();
	if(pdev == NULL)
	{
		printk("Failed to cdev_alloc.\n");
		goto err;	
	}

	cdev_init(pdev, &fops);

	ret = cdev_add(pdev, devno, count);
	if(ret)
	{
		printk("Failed to cdev_add.\n");
		goto err1;
	}

	// 自动创建设备结点
	
	pclass = class_create(THIS_MODULE, "myclass");
	if(IS_ERR(pclass))
	{
		ret = PTR_ERR(pclass);
		goto err2;
	}

	devicep = device_create(pclass, NULL, devno, NULL, "led");
	if(IS_ERR(devicep))
	{
		ret = PTR_ERR(devicep);
		goto err3;
	}

	gpf3con = ioremap(res0->start, 4);
	if(gpf3con == NULL)
	{
		printk("Failed to ioremap.\n");
		goto err4;
	}

	gpf3dat = ioremap(res1->start, 4);
	if(gpf3dat == NULL)
	{
		printk("Failed to ioremap.\n");
		goto err5;
	}

	// 设置输出模式
	dat = readl(gpf3con);
	dat = (dat & (~(0xf << 20)))|(0x1 << 20);
	writel(dat, gpf3con);

	return 0;

err5:
	iounmap(gpf3con);
err4:
	device_destroy(pclass, devno);
err3:
	class_destroy(pclass);
err2:
	cdev_del(pdev);
err1:
	kfree(pdev);
err:
	unregister_chrdev_region(devno,count);

	return ret;

	
}

int demo_remove(struct platform_device * pdevice)  // 移除函数
{
	printk("%s,%d\n", __func__, __LINE__);

	iounmap(gpf3con);
	iounmap(gpf3dat);

	device_destroy(pclass, devno);
	class_destroy(pclass);
	cdev_del(pdev);
	kfree(pdev);
	unregister_chrdev_region(devno,count);

	return 0;
}

struct platform_device_id  ids[] = {
	{.name = "platform_led", },
	{/*Nothing to be done.*/},
};

struct platform_driver pdriver = {
	.probe = demo_probe,
	.remove = demo_remove,
	.driver = {
		.name = "platform_led",
	},
	.id_table = ids, 
};

module_platform_driver(pdriver);

