#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("SKYFALL");

//用于存放虚拟地址和物理地址
volatile unsigned long virt_addr,phys_addr;
//用户存放三个寄存器的地址
volatile unsigned long *GPL2CON,*GPL2DAT,*GPL2PUD;

void gpl2_device_init(void)
{
	//物理地址的起始地址0x11000100→0x11000108
	phys_addr = 0x11000100;
	//0x11000100是GPL2CON的物理地址
	virt_addr =(unsigned long)ioremap(phys_addr,0x10);
	//指定需要操作的寄存器地址
	GPL2CON = (unsigned long *)(virt_addr+0x00);
	GPL2DAT = (unsigned long *)(virt_addr+0x04);
	GPL2PUD	=(unsigned long *)(virt_addr+0x08);
}
//配置开发板的GPIO寄存器
void gpl2_configure(void)
{
	//配置为输出模式。bit3 bit2 bit1设置为0，bit0设置为1
	*GPL2CON  &= 0xfffffff1;
	*GPL2CON  |= 0x00000001;
	//GPL2PUD寄存器，bit[0:1]设为0x03,上拉模式
	*GPL2PUD |=0x0003;
}
void gpl2_on(void)//点亮led灯
{
	*GPL2DAT |= 0x01;
}
void gpl2_off(void)//灭掉led
{
	*GPL2DAT &= 0xfe;
}
static int led_gpl2_init(void)
{
	printk("init!\n");
	gpl2_device_init();//实现IO内存的映射
	gpl2_configure();//配置GPL2为输出模式
	gpl2_on();
	printk("led gpl2 open\n");
	return 0;
}


static void led_gpl2_exit(void)
{
	printk("exit!\n");
	gpl2_off();
	
	printk("led gpl2 close\n");
}

module_init(led_gpl2_init);
module_exit(led_gpl2_exit);
