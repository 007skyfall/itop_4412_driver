#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/semaphore.h>

#define NAME "semaphore_demo"
int flag = 0;
struct semaphore sem;

int demo_open(struct inode *inode, struct file *file)
{
//int down_trylock(struct semaphore *sem)
	if(down_trylock(&sem)){
		printk("can not access...\n");
		return -EBUSY;
	}
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
int demo_close(struct inode *inode, struct file *file)
{
//void up(struct semaphore *sem)
	up(&sem);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

int major = 0 , minor = 0;

struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.release = demo_close,
};
	
int __init demo_init(void)
{
	major = register_chrdev(major,NAME,&f_ops);
	if(major < 0){
		printk("register_chrdev fail ...%s,%d\n",__func__,__LINE__);
		return -EINVAL;
	}
	//static inline void sema_init(struct semaphore *sem, int val)
	sema_init(&sem , 2);//初始化信号量
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

