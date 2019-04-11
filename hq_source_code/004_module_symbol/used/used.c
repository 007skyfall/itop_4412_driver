#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include "../symbol.h"

MODULE_LICENSE("GPL");

extern struct object obj;

static int __init demo_init(void)
{
	printk("%s,%d\n", __func__, __LINE__);
	printk("val:%d\n", obj.val);

	obj.func();

	return 0;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n", __func__, __LINE__);
}

module_init(demo_init);
module_exit(demo_exit);

