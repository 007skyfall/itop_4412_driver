#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/io.h>


MODULE_LICENSE("GPL");

#define  GPF3CON  0x114001E0
#define  GPF3DAT  0x114001E4

dev_t devno;
int minor = 0;
int count = 1;

struct cdev *pdev;
struct class *pclass;
struct device *pdevice;

void __iomem * gpf3con;
void __iomem * gpf3dat;


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

static int __init demo_init(void)
{
	int ret = 0;
	unsigned int dat = 0;

	printk("%s,%d\n", __func__, __LINE__);

	ret = alloc_chrdev_region(&devno, minor, count, "module_led");
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

	pdevice = device_create(pclass, NULL, devno, NULL, "module_led");
	if(IS_ERR(pdevice))
	{
		ret = PTR_ERR(pdevice);
		goto err3;
	}
	
	gpf3con = ioremap(GPF3CON, 4);
	if(gpf3con == NULL)
	{
		printk("Failed to ioremap.\n");
		goto err4;
	}

	gpf3dat = ioremap(GPF3DAT, 4);
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

static void __exit demo_exit(void)
{
	printk("%s,%d\n", __func__, __LINE__);

	iounmap(gpf3con);
	iounmap(gpf3dat);

	device_destroy(pclass, devno);
	class_destroy(pclass);
	cdev_del(pdev);
	kfree(pdev);
	unregister_chrdev_region(devno,count);

}

module_init(demo_init);
module_exit(demo_exit);

