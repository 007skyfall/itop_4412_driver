#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/errno.h>
#include <linux/ioport.h>
/*
流程：			
（1）模块三要素		
（2）platfrom_drievr定义 + 填充		
（3）注册、注销
疑问:
	(1)设备端的name与驱动端的哪一个name匹配?
	   platform_device与platform_driver 是根据总线上的匹配规则去完成对应的匹配
	   platform总线->match
*/
struct resource *res1,*res2;
int pdrv_probe(struct platform_device *pdev)
{
	res1 = platform_get_resource(pdev,IORESOURCE_MEM,0);
	if(res1 == NULL){
		printk("%s,%d platform_get_resource fail...\n",__func__,__LINE__);
		return -ENOMEM;	
	}
	printk("res1->start :%x , res1->end :%x\n",res1->start,res1->end);


	res2 = platform_get_resource(pdev,IORESOURCE_IRQ,0);
	if(res2 == NULL){
		printk("%s,%d platform_get_resource fail...\n",__func__,__LINE__);
		return -ENOMEM;	
	}
	printk("res2->start :%x , res2->end :%x\n",res2->start,res2->end);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
int pdrv_remove(struct platform_device *pdev)
{
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

struct of_device_id	of_tables[] = {
	[0] = {.compatible = "1425637"},
	[1] = {/*NULL 表征结束*/}
};

MODULE_DEVICE_TABLE(of,of_tables);

const struct platform_device_id id_tables[] = {
	[0] = {.name = "pdev"},
	[1] = {.name = "pdev1"}
};

struct platform_driver pdrv = {
	.probe = pdrv_probe,
	.remove = pdrv_remove,
	.driver = {
		.name = "pdev",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(of_tables),
	},
	.id_table = id_tables,
};

static int pdrv_init(void)
{
	platform_driver_register(&pdrv);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

static void pdrv_exit(void)
{
	platform_driver_unregister(&pdrv);
	printk("%s,%d\n",__func__,__LINE__);
}

module_init(pdrv_init);
module_exit(pdrv_exit);
MODULE_LICENSE("GPL");


