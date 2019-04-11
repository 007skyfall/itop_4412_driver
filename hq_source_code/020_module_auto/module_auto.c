#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>


MODULE_LICENSE("GPL");

dev_t devno;
int minor = 0;
int count = 1;

struct cdev *pdev;
struct class *pclass;
struct device *pdevice;


int demo_open(struct inode *inodep, struct file *filep)
{
	printk("%s,%d\n", __func__, __LINE__);
	return 0;
}

int demo_release(struct inode *inodep, struct file * filep)
{
	printk("%s,%d\n", __func__, __LINE__);
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

	printk("%s,%d\n", __func__, __LINE__);

	ret = alloc_chrdev_region(&devno, minor, count, "module_auto");
	if(ret)
	{
		printk("Failed to alloc_chrdev_region.\n");
		return ret;
	}
	printk("devno:%d , major:%d  minor:%d\n", devno, MAJOR(devno), MINOR(devno));
	//struct cdev *cdev_alloc(void)分配一个字符设备结构体
	pdev = cdev_alloc();
	if(pdev == NULL)
	{
		printk("Failed to cdev_alloc.\n");
		goto err;	
	}
//void cdev_init(struct cdev *cdev, const struct file_operations *fops) 初始化一个字符设备结构体
	cdev_init(pdev, &fops);
//int cdev_add(struct cdev *p, dev_t dev, unsigned count) 添加一个字符设备
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

	pdevice = device_create(pclass, NULL, devno, NULL, "test_module_auto");
	if(IS_ERR(pdevice))
	{
		ret = PTR_ERR(pdevice);
		goto err3;
	}

	return 0;

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

	device_destroy(pclass, devno);
	class_destroy(pclass);
	cdev_del(pdev);
	kfree(pdev);
	unregister_chrdev_region(devno,count);

}

module_init(demo_init);
module_exit(demo_exit);
