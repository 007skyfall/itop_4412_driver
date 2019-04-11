#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include "../symbol.h"

MODULE_LICENSE("GPL");

int show(void)
{
	printk("show()...");
	return 0;
}

struct object obj = {
	.val = 100,
	.func = show,
};

EXPORT_SYMBOL_GPL(obj);

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

