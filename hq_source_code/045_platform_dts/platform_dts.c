#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");


int demo_probe(struct platform_device * pdevice) // 探测函数，一旦设备与驱动匹配成功，就回调此函数
{
	struct device_node *nodep;
	struct property *proper;
	int lenp = sizeof(int);
	const char *out_string;
	int ret = 0;

	printk("probe ok!\n");

	nodep = of_find_node_by_path("/platform_driver");
	if(nodep == NULL)
	{
		printk("Failed to of_find_node_by_path.\n");
		return -1;
	}

	proper = of_find_property(nodep,"b", &lenp);
	if(proper == NULL)
	{
		printk("Failed to of_find_property.\n");
		return -1;
	}	
	printk("b:%#x\n", be32_to_cpu(*((unsigned int *)proper->value)));

	ret = of_property_read_string(nodep, "a",&out_string);
	if(ret)
	{
		printk("Failed to of_property_read_string.\n");
		return ret;
	}
	printk("a:%s\n", out_string);


	return 0;
}

int demo_remove(struct platform_device * pdevice)  // 移除函数
{
	printk("%s,%d\n", __func__, __LINE__);

	return 0;
}

struct of_device_id  idts[] = {
	{.compatible="fs4412,platform_driver"},
	{/*Nothing to be done*/},
};

struct platform_driver pdriver = {
	.probe = demo_probe,
	.remove = demo_remove,
	.driver = {
		.name = "platform_driver",
		.of_match_table = idts,
	},
};

module_platform_driver(pdriver);

