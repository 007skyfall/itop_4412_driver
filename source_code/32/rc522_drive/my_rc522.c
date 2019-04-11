#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>

struct spi_device *my_spi;

#define RC522_RESET_PIN	EXYNOS4_GPK1(0)
void my_rc522_reset()
{
	//printk("************************ %s\n", __FUNCTION__);
	if(gpio_request_one(RC522_RESET_PIN, GPIOF_OUT_INIT_HIGH, "RC522_RESET"))
                pr_err("failed to request GPK1_0 for RC522 reset control\n");

        s3c_gpio_setpull(RC522_RESET_PIN, S3C_GPIO_PULL_UP);
        gpio_set_value(RC522_RESET_PIN, 0);

        mdelay(5);

        gpio_set_value(RC522_RESET_PIN, 1);
        gpio_free(RC522_RESET_PIN);
}

//static ssize_t rc522_write(unsigned char *buffer, int len)
static ssize_t rc522_write(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int status;
	unsigned char tx_buf[2];
	
	status = copy_from_user(tx_buf,buf,count);
	
	struct spi_transfer	t = {
		.tx_buf		= tx_buf,
		.len		= count,
	};
	struct spi_message	m;
	spi_message_init(&m);
	spi_message_add_tail(&t, &m);
	DECLARE_COMPLETION_ONSTACK(done);
	m.complete = complete;
	m.context = &done;
	
	printk("spi_async send begin!\n");
	status = spi_async(my_spi,&m);
	if(status == 0){
		wait_for_completion(&done);
		status = m.status;
		if (status == 0)
			status = m.actual_length;
	}
	return status;
}


//static ssize_t rc522_read(unsigned char *buffer, int len)
static ssize_t rc522_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int status;
	unsigned char *rx_buf;
	
	struct spi_transfer	t = {
		.rx_buf		= &rx_buf,
		.len		= count,
	};
	struct spi_message	m;
	spi_message_init(&m);
	spi_message_add_tail(&t, &m);
	DECLARE_COMPLETION_ONSTACK(done);
	m.complete = complete;
	m.context = &done;
	
	printk("spi_async read begin!\n");
	status = spi_async(my_spi,&m);
	if(status == 0){
		wait_for_completion(&done);
		status = m.status;
		if (status == 0)
			status = m.actual_length;
	}
	
	status = copy_to_user(buf,&rx_buf,status);
	
	return status;
}

int rc522_open(struct inode *inode,struct file *filp)
{
	return 0;
}



static struct file_operations rc522_ops = {
	.owner 	= THIS_MODULE,
	.open 	= rc522_open,
	.read	= rc522_read,
	.write 	= rc522_write,
};

static struct miscdevice rc522_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.fops	= &rc522_ops,
	.name	= "rc522",
};

static int __devinit my_rc522_probe(struct spi_device *spi)
{
	
	printk("my_rc522_probe!\n");
	
	/* reset */
	my_rc522_reset();
	my_spi = spi;
	
	misc_register(&rc522_dev);
	
	return 0;
}

static int __devexit my_rc522_remove(struct spi_device *spi)
{
	printk("my_rc522_remove!\n");
	misc_deregister(&rc522_dev);	
	return 0;
}

static struct spi_driver my_rc522_spi_driver = {
	.driver = {
		.name  = "my_rc522",
		.owner = THIS_MODULE,
	},
	.probe =	my_rc522_probe,
	.remove = __devexit_p(my_rc522_remove),

	/* NOTE:  suspend/resume methods are not necessary here.
	 * We don't do anything except pass the requests to/from
	 * the underlying controller.  The refrigerator handles
	 * most issues; the controller driver handles the rest.
	 */
};


static int __init my_rc522_init(void)
{
	spi_register_driver(&my_rc522_spi_driver);
	return 0;
}

static void __exit my_rc522_exit(void)
{
	spi_unregister_driver(&my_rc522_spi_driver);
}

module_exit(my_rc522_exit);
module_init(my_rc522_init);


MODULE_AUTHOR("topeet: rty");
MODULE_LICENSE("GPL");