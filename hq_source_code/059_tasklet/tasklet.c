#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_irq.h>
/*
[3]在驱动中编写中断；			
	[3.1]模块三要素			
	[3.2]获取设备节点信息			
	[3.3]获取该节点中的中断信息，并完成对应中断号的映射			
	[3.4]请求中断			
	[3.5]中断处理函数
*/

#define MAX 64
#define NAME "demo"
int count = 1;
int major = 0 , minor = 0;
struct class *cls = NULL;
struct device *dev = NULL;
/******************************/

struct device_node *np = NULL;
int key2_irq = 0;

void task_func(unsigned long data)
{
	int i = 0;
	while(i<100)
		printk("++i:%d , data:%d \n",++i,data);
}

struct tasklet_struct task = {
	.func = task_func,
	.data = 9,
};

irqreturn_t  key2_irq_handler(int irq, void *data)
{
	tasklet_schedule(&task);
	return IRQ_HANDLED;
}

int demo_open(struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
int demo_close(struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.release = demo_close,
};

int __init demo_init(void)
{
	int ret ,i ;
	major = register_chrdev(major,NAME,&f_ops);
	if(major < 0){
		printk("register_chrdev fail ...%s,%d\n",__func__,__LINE__);
		return -EINVAL;
	}
	cls = class_create(THIS_MODULE,"demo_class");
	if(IS_ERR(cls)){
		printk("class_create ...\n");
		ret = PTR_ERR(cls);
		goto ERR_STEP1;
	}
	for(i = minor ; i < minor + count ; i++){
		dev = device_create(cls,NULL,MKDEV(major,i),NULL,"%s%d","demo",i);
		if(IS_ERR(dev)){
			printk("class_create ...\n");
			ret = PTR_ERR(dev);
			goto ERR_STEP2;
		}
	}
	/**************************************************/
	np = of_find_node_by_path("/fs4412-key-int");
	if(np == NULL){
		printk("of_find_node_by_path...%s,%d\n",__func__,__LINE__);
		return -ENOMEM;
	}

	key2_irq = irq_of_parse_and_map(np,0);
	if(key2_irq < 0){
		printk("irq_of_parse_and_map...%s,%d\n",__func__,__LINE__);
		return -EINVAL;
	}

	ret = request_irq(key2_irq,key2_irq_handler,IRQF_TRIGGER_FALLING,"key2",NULL);
	if(ret < 0){
		printk("request_irq...%s,%d\n",__func__,__LINE__);
		return -EINVAL;
	}

	printk("major :%d \t %s,%d\n",major ,__func__,__LINE__);
	return 0;
	
ERR_STEP2:
	for(i-- ; i >= minor ; i--)
		device_destroy(cls,MKDEV(major,i));
	class_destroy(cls);
ERR_STEP1:
	unregister_chrdev(major,NAME);
	return ret;
}

void __exit demo_exit(void)
{
	int i ;
	free_irq(key2_irq,NULL);
	for(i = minor ; i < minor + count ; i++)
		device_destroy(cls,MKDEV(major,i));
	class_destroy(cls);
	unregister_chrdev(major,NAME);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");

