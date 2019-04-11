#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include "cmd.h"


MODULE_LICENSE("GPL");

dev_t devno;
int major = 250;
int minor = 0;
int count = 1;
struct cdev cdev;

#define  KMAX  1024
char kbuf[KMAX] = {};
int counter = 0;//记录kbuf中实际存储的字节数

struct node n1;

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

int mystrcpy(char *dst, char *src, int n)
{
	if(n < 0)
	{
		return -1;
	}
	while(n--)
	{
		*dst = *src;
		dst++;
		src++;
	}

	return 0;
}

// read(fd, buf, N) --> sys_read() --> ... demo_read();
ssize_t demo_read(struct file *filep, char __user *buffer, size_t size, loff_t * offlen)
{
	if(size > counter)
	{
	   size = counter;
	}
	if(copy_to_user(buffer, kbuf, size) != 0)
	{
		printk("Failed to copy_to_user.\n");
		return -1;
	}

	if(counter-size > 0)
	{
		mystrcpy(kbuf,kbuf+size, counter-size);
	}

	counter -= size;
	
	return size;
}

// write(fd, buf, n) --> sys_wrte() --> ... -->demo_write();
ssize_t demo_write(struct file *filep, const char __user *buffer, size_t size, loff_t * offlen)
{
	if(size > KMAX-counter)
	{
		return -ENOMEM;
	}
	if(copy_from_user(kbuf+counter, buffer, size) != 0)
	{
		printk("Failed to copy_from_user.");
		return -1;
	}
	counter += size;
	printk("kbuf:%s\n", kbuf);

	return size;
}

// ioctl(fd, cmd, arg) --> ... --> demo_ioctl();
long demo_ioctl(struct file * filep, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
	case CMD:
		printk("CMD OK!\n");
		break;
	case CMDR:
		printk("CMDR OK!\n");
		n1.val = 300;
		if(copy_to_user((struct node *)arg, &n1, sizeof(struct node)) != 0)
		{
			printk("Failed to copy_to_user.\n");
			return -1;
		}
		break;
	case CMDW:
		if(copy_from_user(&n1, (struct node *)arg, sizeof(struct node)) != 0)
		{
			printk("Failed to copy_from_user.\n");
			return -1;
		}
		printk("CMDW OK!\n");
		printk("val:%d ,%s\n", n1.val, n1.buf);
		break;
	default:
		printk("Invalid cmd.\n");
		return -1;
	}

	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.release = demo_release,
	.read = demo_read,
	.write = demo_write,
	.unlocked_ioctl = demo_ioctl,
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
