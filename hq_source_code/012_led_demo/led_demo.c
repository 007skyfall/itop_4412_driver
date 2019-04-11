#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/io.h>
/*
���̣�			
��1������Ӳ��ԭ��ͼ		
�����led5  ---->  gpf3_5   ---> ���ƣ�+   ��ƣ�-				
��2��������Ӳ���������õļĴ���				
�����ַ��				
con:0x114001e0	[20-23]   1(output)			
dat:0x114001e4  [5]		  1��   0��								
��3���ģ���� + cdev 					
	���̣�							
	1.ģ����Ҫ��						
	2.����ַ��豸��صĲ���	
	3.���Ӳ���Ĳ���
		3.1 ���ӳ��
		3.2 ���üĴ���
*/
#define NAME "led_demo"
#define GPF3CON 	0x114001e0
#define GPF3DAT		0x114001e4

void __iomem *gpf3con;
void __iomem *gpf3dat;

int major = 0;

int led_open(struct inode *inode, struct file *file)
{
	//����LED��
	writel((0x1<<5),gpf3dat);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
int led_release(struct inode *inode, struct file *file)
{
	//Ϩ��LED��
	writel(~(0x1<<5),gpf3dat);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.release = led_release,

};
int led_init(void)
{
	int ret = 0;
	/*
	static inline int register_chrdev(unsigned int major, const char *name,
				  const struct file_operations *fops)
	*/
	major = register_chrdev(major,NAME,&f_ops);
	if(major < 0){
		printk("%s,%dregister_chrdev fail...\n",__func__,__LINE__);
		return -EINVAL;
	}
	//ioremap (unsigned long phys_addr, unsigned long size)
	gpf3con = ioremap(GPF3CON,4);
	if(gpf3con == NULL){
		printk("%s,%d ioremap gpf3con fail...\n",__func__,__LINE__);
		goto ERR_STEP1;
	}
	gpf3dat = ioremap(GPF3DAT,4);
	if(gpf3dat == NULL){
		printk("%s,%d ioremap gpf3dat fail...\n",__func__,__LINE__);
		goto ERR_STEP2;
	}
	/*
	static inline u32 readl(const volatile void __iomem *addr)
	static inline void writel(unsigned int b, volatile void __iomem *addr)
	*/
	writel((readl(gpf3con)&~(0xf<<20))|(0x1<<20),gpf3con);
	
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
ERR_STEP2:
	//void iounmap(void *addr)
	iounmap(gpf3con);
ERR_STEP1:
	//static inline void unregister_chrdev(unsigned int major, const char *name)
	unregister_chrdev(major,NAME);
	return ret;
}

void led_exit(void)
{
	iounmap(gpf3con);
	iounmap(gpf3dat);
	unregister_chrdev(major,NAME);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");

