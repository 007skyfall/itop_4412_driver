#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


MODULE_LICENSE("GPL");

int __init init_module(void)
{
	printk("%s,%d\n", __func__, __LINE__);

	return 0;
}

void __exit cleanup_module(void)
{
	printk("%s,%d\n", __func__, __LINE__);
}


