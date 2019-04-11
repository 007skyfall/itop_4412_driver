#include <linux/init.h>
#include <linux/module.h>

/*驱动注册的头文件，包含驱动的结构体和注册和卸载的函数*/
#include <linux/platform_device.h>
/*注册杂项设备头文件*/
#include <linux/miscdevice.h>
/*注册设备节点的文件结构体*/
#include <linux/fs.h>
//原子操作的函数头文件
#include <asm/atomic.h>
#include <asm/types.h>

#define DRIVER_NAME "atomic_int"
#define DEVICE_NAME "atomic_int"
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("SKYFALL");

//定义原子变量,并初始化为0
static atomic_t value_atomic = ATOMIC_INIT(0);

static int atomic_int_open(struct inode *inode, struct file *file){
	printk(KERN_EMERG "atomic_int open in!\n");	
	
	if(atomic_read(&value_atomic)){
		return -EBUSY;
	}
	
	atomic_inc(&value_atomic);
	
	printk(KERN_EMERG "atomic_int open success!\n");	
	return 0;
}

static int atomic_int_release(struct inode *inode, struct file *file){
	printk(KERN_EMERG "atomic_int release\n");
	
	atomic_dec(&value_atomic);
	
	return 0;
}

static struct file_operations atomic_int_ops = {
	.owner = THIS_MODULE,
	.open = atomic_int_open,
	.release = atomic_int_release,
};


static  struct miscdevice atomic_int_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &atomic_int_ops,
};


static int atomic_int_probe(struct platform_device *pdv){
	
	printk(KERN_EMERG "\tinitialized\n");
	misc_register(&atomic_int_dev);
	
	return 0;
}

static int atomic_int_remove(struct platform_device *pdv){
	
	printk(KERN_EMERG "\tremove\n");
	misc_deregister(&atomic_int_dev);
	return 0;
}

struct platform_driver atomic_int_driver = {
	.probe = atomic_int_probe,
	.remove = atomic_int_remove,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};


static int atomic_int_init(void)
{
	int DriverState;
	
	printk(KERN_EMERG "HELLO WORLD enter!\n");
	DriverState = platform_driver_register(&atomic_int_driver);
	
	printk(KERN_EMERG "\tDriverState is %d\n",DriverState);
	return 0;
}


static void atomic_int_exit(void)
{
	printk(KERN_EMERG "HELLO WORLD exit!\n");
	
	platform_driver_unregister(&atomic_int_driver);	
}

module_init(atomic_int_init);
module_exit(atomic_int_exit);

