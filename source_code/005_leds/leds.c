#include <linux/init.h>
#include <linux/module.h>

/*驱动注册的头文件，包含驱动的结构体和注册和卸载的函数*/
#include <linux/platform_device.h>
/*注册杂项设备头文件*/
#include <linux/miscdevice.h>
/*注册设备节点的文件结构体*/
#include <linux/fs.h>

/*Linux中申请GPIO的头文件*/
#include <linux/gpio.h>
/*三星平台的GPIO配置函数头文件*/
/*三星平台EXYNOS系列平台，GPIO配置参数宏定义头文件*/
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
/*三星平台4412平台，GPIO宏定义头文件*/
#include <mach/gpio-exynos4.h>

#define DRIVER_NAME "hello_ctl"
#define DEVICE_NAME "hello_gpio"


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("SKYFALL");




/*led的两个IO，网络是KP_COL0，VDD50_EN*/
/*蜂鸣器的1个IO，网络是MOTOR_PWM*/

/*矩阵键盘的8个IO，网络是CHG_FLT，HOOK_DET，CHG_UOK，XEINT14_BAK，
GM_INT1，6260_GPIO1，CHG_COK，XEINT29/KP_ROW13/ALV_DBG25*/

/*摄像头的14个IO，网络是CAM_MCLK，CAM2M_RST，CAM2M_PWDN，
CAM_D5，CAM_D7，CAM_D6，CAM_D4，CAM_D3，CAM_D2，CAM_D1，
CAM_PCLK，CAM_D0，CAM_VSYNC，CAM_HREF。
I2C_SDA7，I2C_SCL7也是可以设置为GPIO，不过总线一般不要去动它*/

/*WIFI模块的7个IO，WIFI_D3，WIFI_CMD，WIFI_D1，WIFI_CLK，WIFI_D0，WIFI_D2，GPC1_1*/
/*串口RX和TX等也是可以设置为GPIO，一般不要动它*/

/*数组中有32个引出到端子或者模块的IO，还有类似sd卡等也是可以作为GPIO，
其它引到连接器但是没有使用的GPIO等等*/
/*SCP管脚编号和POP的稍微有点不同，下面是SCP的*/
static int led_gpios[] = {
	EXYNOS4_GPL2(0),EXYNOS4_GPK1(1),
	EXYNOS4_GPD0(0),
	
	EXYNOS4_GPX1(0),EXYNOS4_GPX1(3),EXYNOS4_GPX1(5),EXYNOS4_GPX1(6),
	EXYNOS4_GPX3(0),EXYNOS4_GPX2(6),EXYNOS4_GPX2(7),EXYNOS4_GPX3(5),
	
	EXYNOS4212_GPJ1(3),EXYNOS4_GPL0(1),EXYNOS4_GPL0(3),EXYNOS4212_GPJ1(0),
	EXYNOS4212_GPJ1(2),EXYNOS4212_GPJ1(1),EXYNOS4212_GPJ0(7),EXYNOS4212_GPJ0(6),
	EXYNOS4212_GPJ0(5),EXYNOS4212_GPJ0(4),EXYNOS4212_GPJ0(0),EXYNOS4212_GPJ0(3),
	EXYNOS4212_GPJ0(1),EXYNOS4212_GPJ0(2),
	
	EXYNOS4_GPK3(6),EXYNOS4_GPK3(1),EXYNOS4_GPK3(4),EXYNOS4_GPK3(0),
	EXYNOS4_GPK3(3),EXYNOS4_GPK3(5),EXYNOS4_GPC1(1),
};

//#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define LED_NUM		ARRAY_SIZE(led_gpios)


static long hello_ioctl( struct file *files, unsigned int cmd, unsigned long arg){
	printk("cmd is %d,arg is %d\n",cmd,arg);
	
	switch(cmd)
	{
		case 0:
		case 1:
			if (arg > LED_NUM) {
				return -EINVAL;
			}

			gpio_set_value(led_gpios[arg], cmd);
			break;

		default:
			return -EINVAL;
	}
	
	gpio_set_value(led_gpios[2], 0);
	
return 0;
}

static int hello_release(struct inode *inode, struct file *file){

	printk("%s,%d\n",__func__,__LINE__);
	printk(KERN_EMERG "hello release\n");
	return 0;
}

static int hello_open(struct inode *inode, struct file *file){
	
	printk("%s,%d\n",__func__,__LINE__);
	printk(KERN_EMERG "hello open\n");
	return 0;
}

static struct file_operations hello_ops = {
	.owner = THIS_MODULE,
	.open = hello_open,
	.release = hello_release,
	.unlocked_ioctl = hello_ioctl,
};

static  struct miscdevice hello_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &hello_ops,
};


static int hello_probe(struct platform_device *pdv){
	int ret,i;
	
	printk(KERN_EMERG "\tinitialized\n");
	
	for(i=0; i<LED_NUM; i++)
	{
		ret = gpio_request(led_gpios[i], "LED");
		if (ret) {
			printk("%s: request GPIO %d for LED failed, ret = %d\n", DRIVER_NAME,
					i, ret);
			}
		else{
			s3c_gpio_cfgpin(led_gpios[i], S3C_GPIO_OUTPUT);
			gpio_set_value(led_gpios[i], 1);			
		}
	}
//单独将蜂鸣器的关掉，为的是不让它开始鸣叫，要不太吵！
	gpio_set_value(led_gpios[2], 0);
	
	printk("%s,%d\n",__func__,__LINE__);
	misc_register(&hello_dev);
	if(ret<0)
	{
		printk("leds:register device failed!\n");
		goto exit;
	}
	
exit:
	misc_deregister(&hello_dev);
	return ret;
return 0;
}

static int hello_remove(struct platform_device *pdv){
	int i;
	
	printk(KERN_EMERG "\tremove\n");
	
	for(i=0; i<LED_NUM; i++)
	{
		gpio_free(led_gpios[i]);
	}
	
	misc_deregister(&hello_dev);
	return 0;
}

static void hello_shutdown(struct platform_device *pdv){
	
	;
}

static int hello_suspend(struct platform_device *pdv,pm_message_t pmt){
	
	return 0;
}

static int hello_resume(struct platform_device *pdv){
	
	return 0;
}

struct platform_driver hello_driver = {
	.probe = hello_probe,
	.remove = hello_remove,
	.shutdown = hello_shutdown,
	.suspend = hello_suspend,
	.resume = hello_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};


static int hello_init(void)
{
	int DriverState;
	printk("%s,%d\n",__func__,__LINE__);
	printk(KERN_EMERG "HELLO WORLD enter!\n");
	DriverState = platform_driver_register(&hello_driver);
	
	printk(KERN_EMERG "\tDriverState is %d\n",DriverState);
	return 0;
}


static void hello_exit(void)
{
	printk("%s,%d\n",__func__,__LINE__);
	printk(KERN_EMERG "HELLO WORLD exit!\n");
	
	void free_irq(unsigned int irq,void *dev_id);
	
	platform_driver_unregister(&hello_driver);	
}

module_init(hello_init);
module_exit(hello_exit);
