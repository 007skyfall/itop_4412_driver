#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/io.h>


#define NAME "led3_demo"
#define GPK1CON 	0x11000060
#define GPK1DAT		0x11000064

void __iomem *gpk1con;
void __iomem *gpk1dat;

int major = 0;

int led_open(struct inode *inode, struct file *file)
{
	printk("%s,%d\n",__func__,__LINE__);

	writel((0x1<<1),gpk1dat);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
int led_release(struct inode *inode, struct file *file)
{
	
	printk("%s,%d\n",__func__,__LINE__);
	writel(~(0x1<<1),gpk1dat);
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
	
	printk("%s,%d\n",__func__,__LINE__);
	int ret = 0;
	major = register_chrdev(major,NAME,&f_ops);
	if(major < 0){
		printk("%s,%d register_chrdev fail...\n",__func__,__LINE__);
		return -EINVAL;
	}

	gpk1con = ioremap(GPK1CON,4);
	
	printk("%s,%d\n",__func__,__LINE__);
	if(gpk1con == NULL){
		printk("%s,%d ioremap gpk1con fail...\n",__func__,__LINE__);
		goto ERR_STEP1;
	}
	gpk1dat = ioremap(GPK1DAT,4);
	if(gpk1dat == NULL){
		printk("%s,%d ioremap gpk1dat fail...\n",__func__,__LINE__);
		goto ERR_STEP2;
	}
	writel((readl(gpk1con)&~(0xf<<0))|(0x1<<1),gpk1con);
	
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
ERR_STEP2:
	iounmap(gpk1con);
ERR_STEP1:
	unregister_chrdev(major,NAME);
	return ret;
}

void beep_exit(void)
{
	iounmap(gpk1con);
	iounmap(gpk1dat);
	
	printk("%s,%d\n",__func__,__LINE__);
	unregister_chrdev(major,NAME);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(beep_init);
module_exit(beep_exit);
MODULE_LICENSE("GPL");


