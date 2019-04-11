#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>



MODULE_LICENSE("GPL");

char *buf1 = NULL;
char *buf2 = NULL;


static int alloc_init(void)
{
	buf1 = kmalloc(100,GFP_KERNEL);
	memset(buf1,0,100);
	strcpy(buf1,"<<< --- Kmalloc Mem OK! --- >>>");
	printk("BUF 1 : %s\n",buf1);
	
	buf2 =__get_free_page(GFP_KERNEL);
	memset(buf2,0,1024);
	strcpy(buf2,"<<<--- Get Free Page OK! --- >>>");
	printk("BUF 2 : %s\n",buf2);
	return 0;
}


static void alloc_exit(void)
{
	kfree(buf1);
	free_page(buf2);
	printk("<<< --- Module Exit! --->>>\n");
}

module_init(alloc_init);
module_exit(alloc_exit);

