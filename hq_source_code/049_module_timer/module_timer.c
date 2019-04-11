#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>


MODULE_LICENSE("GPL");

struct timer_list  mtimer;

void timer_function(unsigned long data)
{
	printk("timeout.\n");
	printk("jiffies:%ld\n", jiffies);
	printk("data:%ld\n", data);

	mod_timer(&mtimer, jiffies + data*HZ);
}

static int __init demo_init(void)
{
	printk("%s,%d\n", __func__, __LINE__);
	printk("Hz:%d\n", HZ);
	printk("jiffies:%ld\n", jiffies);

	mtimer.expires = jiffies + 5*HZ;
	mtimer.function = timer_function;
	mtimer.data = 10;
	init_timer(&mtimer);

	add_timer(&mtimer);

	return 0;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n", __func__, __LINE__);
	del_timer(&mtimer);
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_AUTHOR("skyfall");
MODULE_DESCRIPTION("This is a driver test.");

