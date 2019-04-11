#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/time.h>

/*
���̣�				
��1��ģ����Ҫ��		
��2����init�л�ȡ��ǰϵͳʱ��		
	��ʱ����صĲ�����					
		[1]���� + ��Ա���					
		[2]��ʼ��				
		[3]���							
��3���ڶ�ʱ���������У���ȡ��ǰʱ�䣬����֮ǰ��ȡ��ʱ����Ƚ�	
��4��exitɾ����ʱ��
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

