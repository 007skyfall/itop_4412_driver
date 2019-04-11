#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>//printk
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/fs.h>

/*
1.模块三要素			
2.完成字符设备框架相关的代码cdev
	2.1 设备号申请
	2.2 cdev的申请
	2.3 cdev与struct file_operations 的绑定
	2.4 cdev的注册
*/
#define NAME "alloc_chrdev_region"
dev_t from;
int baseminor = 0;
int count = 1;
struct cdev *my_cdev = NULL;

int demo_open(struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
int demo_release(struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.release = demo_release,
};
	
int demo_init(void)
{
	int ret = -1;
	/* register a range of char device numbers(注册字符设备)
	int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count,
			const char *name)
	*/
	ret = alloc_chrdev_region(&from,baseminor,count,NAME);
	if(ret){
		printk("%s,%d alloc_chrdev_region fail\n",__func__,__LINE__);
		return -EINVAL;
	}

	my_cdev = cdev_alloc();//申请一个字符设备结构体
	if(my_cdev == NULL){
		printk("%s,%d cdev_alloc fail\n",__func__,__LINE__);
		goto ERR_STEP1;
	}
	
//void cdev_init(struct cdev *cdev, const struct file_operations *fops)
	cdev_init(my_cdev,&f_ops);
	
//int cdev_add(struct cdev *p, dev_t dev, unsigned count)
	cdev_add(my_cdev,from,count);
	
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
	
ERR_STEP1:
	//注销字符设备
	unregister_chrdev_region(from,count);
	return ret;
	
}

void demo_exit(void)
{
	cdev_del(my_cdev);
	unregister_chrdev_region(from,count);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");

