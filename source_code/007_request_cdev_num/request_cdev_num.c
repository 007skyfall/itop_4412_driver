#include <linux/init.h>
/*������ʼ���궨���ͷ�ļ�,�����е�module_init��module_exit�ڴ��ļ���*/
#include <linux/module.h>
/*������ʼ������ģ���ͷ�ļ�,�����е�MODULE_LICENSE�ڴ�ͷ�ļ���*/

/*����module_param module_param_array��ͷ�ļ�*/
#include <linux/moduleparam.h>
/*����module_param module_param_array��perm��ͷ�ļ�*/
#include <linux/stat.h>

/*�����ַ��豸����*/
#include <linux/fs.h>
/*MKDEVת���豸���������͵ĺ궨��*/
#include <linux/kdev_t.h>
/*�����ַ��豸�Ľṹ��*/
#include <linux/cdev.h>

#define DEVICE_NAME "sscdev"
#define DEVICE_MINOR_NUM 2
#define DEV_MAJOR 0
#define DEV_MINOR 0

MODULE_LICENSE("Dual BSD/GPL");
/*�����ǿ�Դ�ģ�û���ں˰汾����*/
MODULE_AUTHOR("SKYFALL");
/*��������*/

int numdev_major = DEV_MAJOR;
int numdev_minor = DEV_MINOR;

/*�������豸��*/
module_param(numdev_major,int,S_IRUSR);
/*������豸��*/
module_param(numdev_minor,int,S_IRUSR);


static int scdev_init(void)
{
	int ret = 0;
	dev_t num_dev;
	
	printk("%s,%d\n",__func__,__LINE__);
	printk(KERN_EMERG "numdev_major is %d!\n",numdev_major);
	printk(KERN_EMERG "numdev_minor is %d!\n",numdev_minor);
	
	if(numdev_major)
	{
		num_dev = MKDEV(numdev_major,numdev_minor);
//int register_chrdev_region(dev_t from, unsigned count, const char *name)
		ret = register_chrdev_region(num_dev,DEVICE_MINOR_NUM,DEVICE_NAME);
	}
	else
	{
		printk(KERN_EMERG "numdev_major %d is failed!\n",numdev_major);
	}
	
	if(ret<0)
	{
		printk(KERN_EMERG "register_chrdev_region req %d is failed!\n",numdev_major);		
	}
	
	printk(KERN_EMERG "scdev_init!\n");
	/*��ӡ��Ϣ��KERN_EMERG��ʾ������Ϣ*/
	return 0;
}

static void scdev_exit(void)
{
	printk(KERN_EMERG "scdev_exit!\n");
	unregister_chrdev_region(MKDEV(numdev_major,numdev_minor),DEVICE_MINOR_NUM);
}


module_init(scdev_init);
/*��ʼ������*/
module_exit(scdev_exit);
/*ж�غ���*/
