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

#include "spidev_test.h"
#include "spidev.h"

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


static int write_test(unsigned char *buffer, int len)
{
	int status;
	struct spi_transfer	t = {
		.tx_buf		= buffer,
		.len		= len,
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

static int read_test(unsigned char *buffer, int len)
{
	int status;
	struct spi_transfer	t = {
		.rx_buf		= buffer,
		.len		= len,
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
	return status;
}

static unsigned char ReadRawRC(int addr)
{
	int ret;
	unsigned char  ReData;
	unsigned char  Address;
	
	Address  = (unsigned char)addr << 1;
	Address |= (1 << 7);
	Address &= ~(1 << 0);
	
	ret = write_test(&Address, 1);
	if (ret < 0)
		printk("spi:SPI Write error\n");

	udelay(100);

	ret = read_test(&ReData, 1);
	if (ret < 0)
		printk("spi:SPI Read error\n");

	return ReData;
}

static int WriteRawRC(int addr, int data)
{
	int ret;
	unsigned char  TxBuf[2];

	//bit7:MSB=0,bit6~1:addr,bit0:RFU=0
	TxBuf[0] = ((unsigned char)addr << 1)&0x7E;
	//TxBuf[0] &= 0x7E;

	TxBuf[1] = (unsigned char)data;
	
	ret = write_test(TxBuf, 2);
	
	if (ret < 0)
		printk("spi:SPI Write error\n");

	udelay(10);

	return ret;
}


static int rc522_init()
{
	int ret;
	char version = 0;

	//reset
	WriteRawRC(CommandReg, PCD_RESETPHASE);
	udelay(10);
	WriteRawRC(ModeReg, 0x3D);
	WriteRawRC(TReloadRegL, 30);
	WriteRawRC(TReloadRegH, 0);
	WriteRawRC(TModeReg, 0x8D);
	WriteRawRC(TPrescalerReg, 0x3E);

	version = ReadRawRC(VersionReg);
	printk("Chip Version: 0x%x\n", version);

	return 0;
}

static int __devinit my_rc522_probe(struct spi_device *spi)
{
	
	printk("my_rc522_probe!\n");
	
	/* reset */
	my_rc522_reset();
	my_spi = spi;
	rc522_init();
	
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