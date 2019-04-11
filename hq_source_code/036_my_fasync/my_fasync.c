#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/wait.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/poll.h>
/*
��1��ģ����Ҫ��			
��2��ʵ���ַ�������
	2.1 �����豸��
	2.2 ����cdev 
	2.3 ��cdev��f_ops
	2.4 cdevע��
��3�����壬����ں�����ʵ�ֵȴ�����ͷ�ĳ�ʼ��		
��4��read���������㣬ֱ��ȥ˯�ߵȴ���������뵽��Ӧ�ĵȴ�������wait_event_interruptible
��5������������ʱ�����ȴ����л���
*/
#define NAME "my_fasync"
#define MAX 128
dev_t from;
int baseminor = 0, count = 3;
int major = 0;
struct cdev *my_cdev = NULL;
struct class *my_class = NULL;
struct device *my_device = NULL;
struct fasync_struct *fasync;

wait_queue_head_t wq;
int flag  = 0;//read������������ flag = 0  ��֮Ϊ1
char ker_buf[MAX] = {0};
int my_open(struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
/*
��1�����struct file_operations->fasync			
��2����fasync�����е���fasync_helper��Ӧ�ò���������		
��3�����ײ㷢���������������������ʱ�����Զ������ź�		
��4������Ӧ�ò�����ź���صĵĴ�����ȥִ��
 (5)�����豸���ر�ʱ��Ӧ�ý����е��첽֪ͨ�����ɾ����
*/
int my_fasync(int fd, struct file *file, int on)
{
	return fasync_helper(fd,file,on,&fasync);
}

int my_release (struct inode *inode, struct file *file)
{
	my_fasync(-1,file,0);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

ssize_t my_read(struct file *file, char __user *user, size_t size, loff_t *loff)
{
	if(size > MAX) size = MAX;
	if(flag == 0){   //flag == 0 ���������� ����ȥ���ߵȴ�
		if(file->f_flags  & O_NONBLOCK){ //�����ͬ��������io�򱨴���
			return -EAGAIN;
		}
		if(wait_event_interruptible(wq,flag)){
			return -ERESTARTSYS;
		}
	}
	if(copy_to_user(user,ker_buf,sizeof(ker_buf))){
		printk("%s,%dcopy_to_user fail...\n",__func__,__LINE__);
	}
	flag = 0;
	memset(ker_buf,0,sizeof(ker_buf));
	
	printk("%s,%d\n",__func__,__LINE__);
	return size;
}
ssize_t my_write(struct file *file, const char __user *user, size_t size, loff_t *loff)
{
	if(size > MAX) size = MAX;
	
	if(copy_from_user(ker_buf,user,size)){
		printk("%s,%d copy_from_user fail...\n",__func__,__LINE__);
	}

	flag = 1;
	wake_up(&wq);
	kill_fasync(&fasync,SIGIO,POLL_IN);
	printk("%s,%d\n",__func__,__LINE__);
	return size;
}
/*
(1)ʵ��struct file_operations->poll				
(2)��poll�����У�����poll_wait����������Ӧ������ŵ�poll��		
(3)poll�������ؾ�������롾�������콣�Ƿ�����ps:�Ƿ�ɶ���д��
*/
unsigned int my_poll(struct file *file, struct poll_table_struct *table)
{
	int mask = 0;
	poll_wait(file,&wq,table);
	if(flag == 1)
		mask |= POLLIN;//in������û��ռ���Եģ������û��ռ����ȥ��ȡ�������
	else
		mask |=	POLLOUT;
	return mask;
}


struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,
	.poll = my_poll,
	.fasync = my_fasync,
};
static int my_wait_init(void)
{
	int ret = 0;
	int i = 0;
	ret = alloc_chrdev_region(&from,baseminor,count,NAME);
	if(ret){
		printk("%s,%d alloc_chrdev_region fail....\n",__func__,__LINE__);
		return -EINVAL;
	}
	major = MAJOR(from);
	
	my_cdev = cdev_alloc();
	if(my_cdev == NULL){
		printk("%s,%d cdev_alloc fail....\n",__func__,__LINE__);
		goto ERR_STEP1;
	}

	cdev_init(my_cdev,&f_ops);

	cdev_add(my_cdev,from,count);
	/*-------------------------------------------*/
	my_class = class_create(THIS_MODULE,"my_class");
	if(IS_ERR(my_class)){
		printk("%s,%d class_create fail....\n",__func__,__LINE__);
		ret = PTR_ERR(my_class);
		goto ERR_STEP2;
	}
	for(i = 0 ; i < count ; i++){
		my_device = device_create(my_class,NULL,MKDEV(major,i),NULL,"%s%d",NAME,i);
		if(IS_ERR(my_device)){
			printk("%s,%d device_create fail....\n",__func__,__LINE__);
			ret = PTR_ERR(my_device);
			goto ERR_STEP3;
		}
	}

	/*----------------------------------------*/
	init_waitqueue_head(&wq);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
ERR_STEP3:
	for(i-- ; i >= 0 ; i--){
		device_destroy(my_class,MKDEV(major,i));
	}
	class_destroy(my_class);
ERR_STEP2:
	cdev_del(my_cdev);
ERR_STEP1:
	unregister_chrdev_region(from,count);
	return 0;

}

static void my_wait_exit(void)
{
	int i = 0;
	for(i= 0; i < count; i++){
		device_destroy(my_class,MKDEV(major,i));
	}
	class_destroy(my_class);
	cdev_del(my_cdev);
	unregister_chrdev_region(from,count);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(my_wait_init);
module_exit(my_wait_exit);
MODULE_LICENSE("GPL");
























