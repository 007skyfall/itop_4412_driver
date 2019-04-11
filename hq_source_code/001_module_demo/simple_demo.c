#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


MODULE_LICENSE("GPL");

static int __init demo_init(void)
{
	printk("%s,%d\n", __func__, __LINE__);

	return 0;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n", __func__, __LINE__);
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_AUTHOR("SKYFALL");
MODULE_DESCRIPTION("This is a simple driver test.");
