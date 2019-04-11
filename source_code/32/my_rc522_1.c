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

static int __devinit my_rc522_probe(struct spi_device *spi)
{
	/* reset */
	//my_rc522_reset();
	printk("my_rc522_probe!\n");
	
	return 0;
}

static int __devexit my_rc522_remove(struct spi_device *spi)
{
	printk("my_rc522_remove!\n");
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
