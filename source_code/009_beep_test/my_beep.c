#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/io.h>
/*
流程：			
（1）分析硬件原理图		
控制beep  ---->  gpd3_0   ---> 响：+    灭：-				
（2）分析该硬件所需配置的寄存器				
物理地址：				
con:0x114001a0	[0-3]   1(output)			
dat:0x114001a4  [0]		  1响   0灭								
（3）搭建模块框架 + cdev 					
	流程：							
	1.模块三要素						
	2.完成字符设备相关的操作	
	3.相关硬件的操作
		3.1 完成映射
		3.2 配置寄存器
*/
#define NAME "beep_demo"
#define GPD0CON 	0x114000a0
#define GPD0DAT		0x114000a4

void __iomem *gpd0con;
void __iomem *gpd0dat;

int major = 0;

int led_open(struct inode *inode, struct file *file)
{
	writel((0x1<<0),gpd0dat);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
int led_release(struct inode *inode, struct file *file)
{
	writel(~(0x1<<0),gpd0dat);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.release = led_release,

};
int beep_init(void)
{
	int ret = 0;
	major = register_chrdev(major,NAME,&f_ops);
	if(major < 0){
		printk("%s,%d register_chrdev fail...\n",__func__,__LINE__);
		return -EINVAL;
	}

	gpd0con = ioremap(GPD0CON,4);
	if(gpd0con == NULL){
		printk("%s,%d ioremap gpd0con fail...\n",__func__,__LINE__);
		goto ERR_STEP1;
	}
	gpd0dat = ioremap(GPD0DAT,4);
	if(gpd0dat == NULL){
		printk("%s,%d ioremap gpd0dat fail...\n",__func__,__LINE__);
		goto ERR_STEP2;
	}
	//writel((readl(gpf0con)&~(0xf<<20))|(0x1<<20),gpf0con);
	writel((readl(gpd0con)&~(0xf<<0))|(0x1<<0),gpd0con);
	
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
ERR_STEP2:
	iounmap(gpd0con);
ERR_STEP1:
	unregister_chrdev(major,NAME);
	return ret;
}

void beep_exit(void)
{
	iounmap(gpd0con);
	iounmap(gpd0dat);
	unregister_chrdev(major,NAME);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(beep_init);
module_exit(beep_exit);
MODULE_LICENSE("GPL");






















