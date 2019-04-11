#include <linux/init.h>
/*������ʼ���궨���ͷ�ļ�,�����е�module_init��module_exit�ڴ��ļ���*/
#include <linux/module.h>
/*������ʼ������ģ���ͷ�ļ�,�����е�MODULE_LICENSE�ڴ�ͷ�ļ���*/

/*����module_param module_param_array��ͷ�ļ�*/
#include <linux/moduleparam.h>
/*����module_param module_param_array��perm��ͷ�ļ�*/
#include <linux/stat.h>


MODULE_LICENSE("Dual BSD/GPL");
/*�����ǿ�Դ�ģ�û���ں˰汾����*/

MODULE_AUTHOR("SKYFALL");
/*��������*/

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
	/*��ӡ��Ϣ��KERN_EMERG��ʾ������Ϣ*/
	return 0;
}

static void hello_exit(void)
{
	
	printk("%s,%d\n",__func__,__LINE__);
	printk(KERN_EMERG "Hello world exit!\n");
}


module_init(hello_init);
/*��ʼ������*/
module_exit(hello_exit);
/*ж�غ���*/
