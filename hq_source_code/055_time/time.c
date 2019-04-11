#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/time.h>

/*
流程：				
（1）模块三要素		
（2）在init中获取当前系统时间		
	定时器相关的操作：					
		[1]定义 + 成员填充					
		[2]初始化				
		[3]添加							
（3）在定时器处理函数中，读取当前时间，并跟之前读取的时间相比较	
（4）exit删除定时器
*/


struct timeval old_val,new_val;
struct timer_list tm;
void tm_function(unsigned long data)
{
	do_gettimeofday(&new_val); 
	printk("new_val - old_val : %ld s\n",(new_val.tv_sec - old_val.tv_sec));

	mod_timer(&tm  , jiffies + 5 * HZ);
	do_gettimeofday(&old_val); 
}

static int time_init(void)
{
	do_gettimeofday(&old_val);


	tm.expires = jiffies + 10 * HZ;
	tm.function = tm_function;
	init_timer(&tm);
	add_timer(&tm);

	
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

static void time_exit(void)
{
	del_timer(&tm);
	printk("%s,%d\n",__func__,__LINE__);
}


module_init(time_init);
module_exit(time_exit);
MODULE_LICENSE("GPL");

