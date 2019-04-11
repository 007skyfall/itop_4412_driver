#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>//printk
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/fs.h>

/*
1.ģ����Ҫ��			
2.����ַ��豸�����صĴ���cdev
	2.1 �豸������
	2.2 cdev������
	2.3 cdev��struct file_operations �İ�
	2.4 cdev��ע��
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
	/* register a range of char device numbers(ע���ַ��豸)
	int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count,
			const char *name)
	*/
	ret = alloc_chrdev_region(&from,baseminor,count,NAME);
	if(ret){
		printk("%s,%d alloc_chrdev_region fail\n",__func__,__LINE__);
		return -EINVAL;
	}

	my_cdev = cdev_alloc();//����һ���ַ��豸�ṹ��
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
	//ע���ַ��豸
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

