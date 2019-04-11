#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

#define NAME "rd_wr_demo"
#define MAX  64
char ker_buf[MAX] = {0};
/*
1.Ä£¿éÈýÒªËØ
2.×Ö·û¿ò¼ÜÏà¹Ø
	2.1 ÉêÇëÉè±¸ºÅ
	2.2 cdevÉêÇë
	2.3 cdev³õÊ¼»¯
	2.4 cdev×¢²á
*/
ssize_t demo_read(struct file *file, char __user *user, size_t size, loff_t *loff)
{
	if(size > MAX)   size = MAX;
	//unsigned long copy_to_user(void *to, const void __user *from, unsigned long n)
	if(raw_copy_to_user(user,ker_buf,size)){
		printk("copy_to_user fail...%s,%d\n",__func__,__LINE__);
		return -EAGAIN;
	}
	
	printk("%s,%d\n",__func__,__LINE__);
	return size;
}

ssize_t demo_write(struct file *file, const char __user *user ,size_t size, loff_t *loff)
{
	if(size > MAX) size = MAX;
//unsigned long copy_from_user(void *to, const void __user *from, unsigned long n)
	if(raw_copy_from_user(ker_buf,user,size)){
		printk("copy_from_user fail...%s,%d\n",__func__,__LINE__);
		return -EAGAIN;
	}
	printk("%s,%d\n",__func__,__LINE__);
	return size;
}

int demo_open (struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
int demo_close (struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

int major = 0 , minor = 0;

struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.release = demo_close,
	.read = demo_read,
	.write = demo_write,
};
	
int __init demo_init(void)
{
	major = register_chrdev(major,NAME,&f_ops);
	if(major < 0){
		printk("register_chrdev fail ...%s,%d\n",__func__,__LINE__);
		return -EINVAL;
	}
	printk("major :%d \t %s,%d\n",major ,__func__,__LINE__);
	return 0;
}

void __exit demo_exit(void)
{
	unregister_chrdev(major,NAME);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");

