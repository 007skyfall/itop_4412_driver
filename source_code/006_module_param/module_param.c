#include <linux/init.h>
/*包含初始化宏定义的头文件,代码中的module_init和module_exit在此文件中*/
#include <linux/module.h>
/*包含初始化加载模块的头文件,代码中的MODULE_LICENSE在此头文件中*/

/*定义module_param module_param_array的头文件*/
#include <linux/moduleparam.h>
/*定义module_param module_param_array中perm的头文件*/
#include <linux/stat.h>


MODULE_LICENSE("Dual BSD/GPL");
/*声明是开源的，没有内核版本限制*/

MODULE_AUTHOR("SKYFALL");
/*声明作者*/

static int module_arg1,module_arg2;
static int int_array[50];
static int int_num;

module_param(module_arg1,int,S_IRUSR);

module_param(module_arg2,int,S_IRUSR);

module_param_array(int_array,int,&int_num,S_IRUSR);


static int hello_init(void)
{
	int i;
	printk("%s,%d\n",__func__,__LINE__);
	
	printk(KERN_EMERG "module_arg1 is %d!\n",module_arg1);
	printk(KERN_EMERG "module_arg2 is %d!\n",module_arg2);
	
	for(i=0;i<int_num;i++){
		printk(KERN_EMERG "int_array[%d] is %d!\n",i,int_array[i]);
	}
		
	printk(KERN_EMERG "Hello World enter!\n");
	/*打印信息，KERN_EMERG表示紧急信息*/
	return 0;
}

static void hello_exit(void)
{
	
	printk("%s,%d\n",__func__,__LINE__);
	printk(KERN_EMERG "Hello world exit!\n");
}


module_init(hello_init);
/*初始化函数*/
module_exit(hello_exit);
/*卸载函数*/
