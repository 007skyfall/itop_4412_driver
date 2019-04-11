#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/slab.h>
static struct timer_list tm;	//�����ں˶�ʱ���ṹ��
static struct timeval oldtime;  //������ʼʱ��

//�ں˶�ʱ������ʱ�����Ҫִ�еĺ�����data�Ǹ��ɺ������ݵĲ�������tm�ṹ��ĳ�Ա
void timer_function(unsigned long data) 
{
	struct timeval newtime;
	do_gettimeofday(&newtime);
	printk("%s\t%ld\n",(char*)data,(newtime.tv_sec-oldtime.tv_sec));
	oldtime=newtime;
	mod_timer(&tm,jiffies + 2*HZ);		        	//���޸Ķ�ʱ��ʱ��
}

void *mem = NULL;
void *mem1 = NULL;
static int __init timer_init(void)//ģ���ʼ��
{
	printk("%s\t%d\n",__func__,__LINE__);
	tm.expires = jiffies + 2*HZ; //��䶨ʱ���ṹ�������Ա
	tm.function = timer_function;
	tm.data = (unsigned long)"hello world";
	init_timer(&tm);
	add_timer(&tm);
	printk("mem:%p,mem1:%p\n",mem,mem1);
	do_gettimeofday(&oldtime);//��ȡ��ǰʱ��
	return 0;
}

static void __exit timer_exit(void)//ģ��ж��
{
	printk("%s��%d\n",__func__,__LINE__);
	del_timer(&tm);	            	//ɾ����ʱ��
}

module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");

