#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "cmd.h"

MODULE_LICENSE("GPL");

#define  KMAX  1024

dev_t devno;
int major = 250;
int minor = 0;
int count = 1;

struct cdev cdev;

char kbuf[KMAX] = {};
int counter = 0;  // 记录当前kbuf中，实际存在的字节个数

int demo_open(struct inode *inodep, struct file * filep)  // 打开文件
{
	printk("%s,%d\n", __func__, __LINE__);
	return 0;
}

int demo_release(struct inode *inodep, struct file * filep) // 关闭文件
{
	printk("%s,%d\n", __func__, __LINE__);
	return 0;
}

int mystrcpy(char *to , char *src, int n)
{
	if(n < 0)
	{
		return -1;
	}
	while(n--)
	{
		*to = *src;
		src++;
		to++;
	}

	return 0;
}

// read(fd, buf, N) --> sys_read() --> ... --> demo_read()
ssize_t demo_read(struct file * filep, char __user * buffer, size_t size, loff_t * offlen)
{
	if(size > counter)
	{
		size = counter;
	}
	if(raw_copy_to_user(buffer, kbuf, size))
	{
		printk("Failed to copy_to_user.\n");
		return -1;
	}

	mystrcpy(kbuf, kbuf+size, counter-size);

	counter -= size;

	return size;
}

// write(fd, buf, n) --> sys_write() --> ... ---> demo_write();
ssize_t demo_write(struct file * filep, const char __user * buffer, size_t size, loff_t * offlen)
{
	if(size > KMAX - counter)
	{
		return -ENOMEM;
	}
	
	if(raw_copy_from_user(kbuf + counter, buffer, size) != 0)
	{
		printk("Failed to copy_from_user.\n");
		return -1;
	}

	counter += size;
	printk("kbuf:%s\n", kbuf);

	return  size;
}

// ioctl(fd, cmdw, &n1) --> ... --> demo_ioctl(filep, cmd, arg);
long demo_ioctl(struct file * filep, unsigned int cmd, unsigned long arg)
{
	static struct node nodeinfo;

	switch(cmd)
	{
	case CMD:
		printk("CMD OK!\n");
		break;
	case CMDR:
		printk("CMDR OK!\n");
		nodeinfo.val = 300;
		mystrcpy(nodeinfo.buf, "This is a CMDR test.", 20);
		if(raw_copy_to_user((struct node *)arg, &nodeinfo, sizeof(nodeinfo)))
		{
			printk("Failed to copy_to_user.\n");
			return -1;
		}
		break;
	case CMDW:
		printk("CMDW OK!\n");
		if(raw_copy_from_user(&nodeinfo, (struct node *)arg, sizeof(struct node)))
		{
			printk("Failed to copy_from_user.\n");
			return -1;
		}
		printk("val:%d --> buf:%s\n", nodeinfo.val, nodeinfo.buf);
		break;
	default:
		printk("cmd is invalid.\n");
		return -EINVAL;	
	}

	return 0;
}

struct file_operations  fops = {
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

	ret = register_chrdev_region(devno, count, "module_ioctl");
	if(ret)
	{
		printk("Failed to register_chrdev_region.\n");
		return ret;
	}

	cdev_init(&cdev, &fops);

	ret = cdev_add(&cdev, devno, count);
	if(ret)
	{
		printk("Failed to cdev_add.");
		goto err;
	}

	return 0;

err:
	unregister_chrdev_region(devno, count);

	return ret;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n", __func__, __LINE__);

	cdev_del(&cdev);
	unregister_chrdev_region(devno, count);
}

module_init(demo_init);
module_exit(demo_exit);

