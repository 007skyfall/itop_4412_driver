#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


MODULE_LICENSE("GPL");

struct node {
	int a;
	char ch;
	short b;
	int c;
	char d;
};


static int __init demo_init(void)
{
	struct node info;
	struct node *p;

	printk("%s,%d\n", __func__, __LINE__);
	
	info.a = 100;
	info.ch = 'c';
	info.b = 20;
	info.c = 300;
	info.d = 'A';

//	p = container_of(&(info.c), struct node, c);
	p = container_of(&(info.a),struct node, a);
	printk("&info:%p , p:%p\n", &info, p);
	printk("a:%d ch:%c b:%d c:%d d:%c\n", p->a, p->ch, p->b, p->c , p->d);


	return 0;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n", __func__, __LINE__);

}

module_init(demo_init);
module_exit(demo_exit);

MODULE_AUTHOR("skyfall");
MODULE_DESCRIPTION("This is a driver test.");
