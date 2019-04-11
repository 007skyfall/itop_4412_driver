#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/slab.h>
static struct timer_list tm;	//定义内核定时器结构体
static struct timeval oldtime;  //定义起始时间

//内核定时器到达时间后，需要执行的函数，data是给干函数传递的参数，是tm结构体的成员
void timer_function(unsigned long data) 
{
	struct timeval newtime;
	do_gettimeofday(&newtime);
	printk("%s\t%ld\n",(char*)data,(newtime.tv_sec-oldtime.tv_sec));
	oldtime=newtime;
	mod_timer(&tm,jiffies + 2*HZ);		        	//将修改定时器时间
}

void *mem = NULL;
void *mem1 = NULL;
static int __init timer_init(void)//模块初始化
{
	printk("%s\t%d\n",__func__,__LINE__);
	tm.expires = jiffies + 2*HZ; //填充定时器结构体各个成员
	tm.function = timer_function;
	tm.data = (unsigned long)"hello world";
	init_timer(&tm);
	add_timer(&tm);
	printk("mem:%p,mem1:%p\n",mem,mem1);
	do_gettimeofday(&oldtime);//获取当前时间
	return 0;
}

static void __exit timer_exit(void)//模块卸载
{
	printk("%s，%d\n",__func__,__LINE__);
	del_timer(&tm);	            	//删除定时器
}

module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");

