#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/io.h>
/*
���̣�			
��1��ģ����Ҫ��		
��2��platfrom_drievr���� + ���		
��3��ע�ᡢע��
����:
	(1)�豸�˵�name�������˵���һ��nameƥ��?
	   platform_device��platform_driver �Ǹ��������ϵ�ƥ�����ȥ��ɶ�Ӧ��ƥ��
	   platform����->match
*/
struct resource *res1,*res2;
void __iomem *gpf3con;
void __iomem *gpf3dat;

int pdrv_probe(struct platform_device *pdev)
{
	res1 = platform_get_resource(pdev,IORESOURCE_MEM,0);//con
	if(res1 == NULL){
		printk("%s,%d platform_get_resource fail...\n",__func__,__LINE__);
		return -ENOMEM;	
	}
	printk("res1->start :%x , res1->end :%x\n",res1->start,res1->end);


	res2 = platform_get_resource(pdev,IORESOURCE_MEM,1);//dat
	if(res2 == NULL){
		printk("%s,%d platform_get_resource fail...\n",__func__,__LINE__);
		return -ENOMEM;	
	}
	printk("res2->start :%x , res2->end :%x\n",res2->start,res2->end);

	gpf3con = ioremap(res1->start,4);
	if(gpf3con == NULL){
		printk("%s,%d ioremap gpf3con fail...\n",__func__,__LINE__);
		return -ENOMEM;	
	}
	gpf3dat = ioremap(res2->start,4);
	if(gpf3dat == NULL){
		printk("%s,%d ioremap gpf3dat fail...\n",__func__,__LINE__);
		return -ENOMEM;	
	}

	writel((readl(gpf3con)&~(0xf<<20))|(0x1<<20),gpf3con);//gpf3_5 output 

	writel((0x1<<5),gpf3dat);//on
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}
int pdrv_remove(struct platform_device *pdev)
{
	writel(~(0x1<<5),gpf3dat);//off 
	iounmap(gpf3con);
	iounmap(gpf3dat);
	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

struct of_device_id	of_tables[] = {
	[0] = {.compatible = "1425637"},
	[1] = {/*NULL ��������*/}
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


