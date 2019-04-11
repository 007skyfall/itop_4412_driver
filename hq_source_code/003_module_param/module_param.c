#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


MODULE_LICENSE("GPL");

int intval = 0;
module_param(intval, int, 0664);
MODULE_PARM_DESC(intval, "This is a integer.");

char *p = NULL;
module_param(p, charp, 0664);

int array[4] = {};
int num = 4;
module_param_array(array, int, &num, 0444);

char str[4] ={};
//#define module_param_string(_name_, _string_, _size_, _perm_) \
//MODULE_PARM(_string_, "c" __MODULE_STRING(_size_))
//	module_param_string(传递参数时变量名称, 数组名, 数组大小, 权限);
module_param_string(estr,str,sizeof(str), 0664);
MODULE_PARM_DESC(estr, "This is a char str[].");

static int __init demo_init(void)
{
	int i = 0;

	printk("%s,%d\n", __func__, __LINE__);
	printk("intval:%d\n", intval);
	printk("p:%s\n", p);

	for(i = 0; i < num; i++)
	{
		printk("array[%d] = %d\n", i, array[i]);
	}

	printk("str:%s\n", str);

	return 0;
}

static void __exit demo_exit(void)
{
	printk("%s,%d\n", __func__, __LINE__);
}

module_init(demo_init);
module_exit(demo_exit);


