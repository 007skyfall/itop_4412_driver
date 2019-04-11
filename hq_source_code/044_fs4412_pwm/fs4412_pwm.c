#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm-generic/uaccess.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/io.h>
#include "cmd.h"

MODULE_LICENSE("GPL");

dev_t devno;
int major = 0;
int minor = 0;
int count = 1;

struct cdev *pdev;
struct device *pdevice;
struct class *mclass;

#define  GPD0CON  0x114000A0
#define  PWMBASE  0x139D0000

#define  TCFG0    0x0000 // 设置预分频
#define  TCFG1    0x0004 // 选择mux通道
#define  TCON     0x0008  // timer控制寄存器
#define  TCNTB0   0x000C  //计数寄存器
#define  TCMPB0   0x0010  //比较寄存器
#define  TCNTO0   0x0014 

void __iomem * pwmbase;
unsigned int * gpd0con;

int fs4412_pwm_ioremap(void)
{
	pwmbase = ioremap(PWMBASE, 24);
	if(pwmbase == NULL)
	{
		printk("Failed to ioremap.\n");
		return -1;
	}

	gpd0con = ioremap(GPD0CON, 4);
	if(gpd0con == NULL)
	{
		printk("Failed to ioremap.\n");
		return -1;
	}
	printk("pwmbase:%p\n", pwmbase);
	printk("gpd0con:%p\n", gpd0con);
	
	return 0;
}

int fs4412_pwm_iounmap(void)
{
	iounmap(pwmbase);
	iounmap(gpd0con);

	return 0;
}

int fs4412_pwm_init(void)
{
	//1,设置TOUT0 模式
	writel((readl(gpd0con)&(~0xf))|0x2, gpd0con);
	printk("gpd0con:%d == 0x2\n", readl(gpd0con));

	//对pwm进行初始化操作
	
	//2,预分频0，[0:7] --> Tout0, 255
	writel(readl(pwmbase+TCFG0)|0xff, pwmbase+TCFG0);
	printk("pwmbase+TCFG0:%p ,%#x \n", pwmbase + TCFG0 ,readl(pwmbase+TCFG0));
	

	//3,选择通道, 1/4
	writel((readl(pwmbase+TCFG1)&(~0xf))|0x2, pwmbase+TCFG1);
	printk("pwmbase+TCFG1:%p ,%#x \n", pwmbase+ TCFG1, readl(pwmbase+TCFG1));


	//4, 设置总计数值为300
	writel(300, pwmbase+TCNTB0);
	printk("pwmbase+TCNTB0:%p ,%#x \n", pwmbase + TCNTB0, readl(pwmbase+TCNTB0));

	//5，设置比较器的值150
	writel(150, pwmbase+TCMPB0);
	printk("pwmbase+TCMPB0:%p ,%#x \n", pwmbase + TCMPB0, readl(pwmbase+TCMPB0));

	//6，设置pwm的控制寄存器, 手动更新
	writel((readl(pwmbase+TCON)&(~0x1f))|0x2, pwmbase+TCON);
	printk("pwmbase+TCON:%p ,%#x \n", pwmbase + TCON, readl(pwmbase+TCON));


	return 0;
}

int demo_open(struct inode *inode, struct file *filep)
{
	printk("%s,%d\n", __func__, __LINE__);

	 return 0;
}

int demo_release(struct inode *inode, struct file *filep)
{
	printk("%s,%d\n", __func__, __LINE__);
	return 0;
}

long demo_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{

	switch(cmd)
	{
	case PWM_ON:
		writel((readl(pwmbase+TCON)&(~0x1f))|0x9, pwmbase+TCON);
		printk("readl:%d\n", readl(pwmbase+TCNTO0));	
		break;
	case PWM_OFF:
		writel((readl(pwmbase+TCON)&(~0x1f)), pwmbase+TCON);
		printk("PWM_OFF.\n");
		break;
	default:
		printk("Invalid data.\n");
		return -1;
	}

	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open =demo_open,
	.release =demo_release,
	.unlocked_ioctl =demo_ioctl,
};

static int __initdemo_init(void)
{
	int ret = 0;

	printk("%s,%d\n", __func__, __LINE__);

	// 申请注册设备号
	ret = alloc_chrdev_region(&devno, minor, count, "fs4412_pwm");
	if(ret)
	{
		printk("Failed to alloc_chrdev_region.\n");
		return ret;
	}

	printk("devno:%d, major:%d , minor:%d\n", devno, MAJOR(devno), MINOR(devno));

	pdev = cdev_alloc();
	if(pdev == NULL)
	{
		printk("Failed to cdev_alloc.\n");
		ret = -ENOMEM;
		goto err;
	}

	cdev_init(pdev, &fops);

	ret = cdev_add(pdev, devno, count);
	if(ret)
	{
		printk("Failed to cdev_add");
		goto err1;
	}

	//自动创建设备结点，代替mknod
	
	mclass = class_create(THIS_MODULE, "myclass");
	if(IS_ERR(mclass))
	{
		ret = PTR_ERR(mclass);
		goto err2;
	}
	pdevice = device_create(mclass, NULL, devno, NULL, "pwm");
	if(IS_ERR(pdevice))
	{
		ret = PTR_ERR(pdevice);
		goto err3;
	}

	fs4412_pwm_ioremap();

	fs4412_pwm_init();

	return 0;

err3:
	class_destroy(mclass);
err2:
	cdev_del(pdev);
err1:
	kfree(pdev);
err:
	unregister_chrdev_region(devno, count);	

	
	return ret;
}

static void __exitdemo_exit(void)
{
	printk("%s,%d\n", __func__, __LINE__);
	fs4412_pwm_iounmap();

	device_destroy(mclass, devno);
	class_destroy(mclass);
	cdev_del(pdev);
	kfree(pdev);
	unregister_chrdev_region(devno, count);	
}

module_init(demon_init);
module_exit(demon_exit);

