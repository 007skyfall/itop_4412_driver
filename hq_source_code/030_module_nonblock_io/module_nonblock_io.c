#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/atomic.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

#define  KMAX  1024

dev_t devno;
int major = 250;
int minor = 0;
int count = 1;

struct cdev cdev;

char kbuf[KMAX] = {};
int counter = 0;  // 记录当前kbuf中，实际存在的字节个数

struct semaphore sem;

wait_queue_head_t  wq;

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

// read(fd, buf, N) --> sys_read() --> ... --> demo_read()
ssize_t demo_read(struct file * filep, char __user * buffer, size_t size, loff_t * offlen)
{

	if(counter == 0)
	{

		if(filep->f_flags & O_NONBLOCK)
		{
			return -EAGAIN;
		}

		if(wait_event_interruptible(wq, counter != 0))
		{
			return -ERESTARTSYS;
		}
	
	}

	down_interruptible(&sem);

	if(size > counter)
	{
		size = counter;
	}
	if(copy_to_user(buffer, kbuf, size))
	{
		printk("Failed to copy_to_user.\n");
		return -1;
	}
	counter = 0;

	return size;
}

// write(fd, buf, n) --> sys_write() --> ... ---> demo_write();
ssize_t demo_write(struct file * filep, const char __user * buffer, size_t size, loff_t * offlen)
{
	if(size > KMAX)
	{
		return -ENOMEM;
	}
	
	if(copy_from_user(kbuf, buffer, size) != 0)
	{
		printk("Failed to copy_from_user.\n");
		return -1;
	}

	counter = size;
	printk("kbuf:%s\n", kbuf);

	up(&sem);

	wake_up(&wq);


	return  size;
}

struct file_operations  fops = {
		.owner = THIS_MODULE,
		.open = demo_open,
		.release = demo_release,
		.read = demo_read,
		.write = demo_write,
};

static int __init demo_init(void)
{
	int ret = 0;

	printk("%s,%d\n", __func__, __LINE__);

	devno = MKDEV(major, minor);
	printk("devno:%d\n", devno);

	ret = register_chrdev_region(devno, count, "module_nonblock_io");
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

	sema_init(&sem, 0);

	init_waitqueue_head(&wq);

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

