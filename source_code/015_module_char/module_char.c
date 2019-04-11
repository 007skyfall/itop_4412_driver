#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>


MODULE_LICENSE("GPL");

dev_t devno;
int major = 250;
int minor = 0;
int count = 1;
struct cdev cdev;

// open() -->sys_open() --> ... --> demo_open()
int demo_open(struct inode *inodep, struct file * filep)
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
	
	devno = MKDEV(major, minor);
	printk("devno:%d\n", devno);

	ret = register_chrdev_region(devno, count, "xxx");
	if(ret)
	{
		printk("Failed to register_chrdev_region.\n");
		return ret;
	}

	cdev_init(&cdev, &fops);

	ret = cdev_add(&cdev, devno, count);
	if(ret)
	{
		unregister_chrdev_region(devno, count);
		printk("Failed to cdev_add.\n");
		return ret;
	}

	return 0;
}

static void __exit demo_exit(void)
{
	cdev_del(&cdev);
	unregister_chrdev_region(devno, count);
	printk("%s,%d\n", __func__, __LINE__);
}

module_init(demo_init);
module_exit(demo_exit);
