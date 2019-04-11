#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/of_irq.h>
#include <linux/of.h>
#include <linux/workqueue.h>
/*
【1】模块三要素		
【2】在入口函数：	
	获取设备信息：struct device_node->irq------------->中断处理函数	
										  request_irq				
	出口：	
		释放irq
*/
struct device_node * np = NULL;
int irq = 0;
/*
void task_func(unsigned long data)
{
	int i = 10;
	while(i--){
		printk("i:%d  data:%ld\n",i,data);
	}
}

struct tasklet_struct task = {
	.func = task_func,
	.data = 9,
};*/
struct work_struct wk;
void work_func(struct work_struct *work)
{
	int i = 10;
	while(i--){
		printk("i:%d \n",i);
	}
}

irqreturn_t key2_irq_handler(int irq, void *data)
{
	printk("%s,%d irq:%d\n",__func__,__LINE__,irq);
	//tasklet_schedule(&task);
	schedule_work(&wk);
	printk("++++++++++++++++++++++++++++++++\n");
	return IRQ_HANDLED;//表征该中断处理成功
}

static int key_init(void)
{
	int ret;
	np = of_find_node_by_path("/key2");
	if(np == NULL){
		printk("%s,%d of_find_node_by_path...\n",__func__,__LINE__);
		return -ENOMEM;
	}

	irq = irq_of_parse_and_map(np,0);
	if(irq <= 0){
		printk("%s,%d irq_of_parse_and_map...\n",__func__,__LINE__);
		return -EINVAL;
	}

	ret = request_irq(irq,key2_irq_handler,IRQF_TRIGGER_FALLING,"key2",NULL);
	if(ret < 0){
		printk("%s,%d request_irq...\n",__func__,__LINE__);
		return -EINVAL;
	}

	INIT_WORK(&wk,work_func);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
static void  key_exit(void)
{
	free_irq(irq,NULL);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(key_init);
module_exit(key_exit);
MODULE_LICENSE("GPL");

