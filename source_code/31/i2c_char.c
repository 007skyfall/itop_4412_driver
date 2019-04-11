#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
#include <linux/regulator/consumer.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <asm/uaccess.h> 
#include <linux/miscdevice.h>

static struct i2c_client *this_client;

static int i2c_tes_read_reg(struct i2c_client *client,u8 addr, u8 *pdata) {
	u8 buf1[4] = { 0 };
	u8 buf2[4] = { 0 };
	struct i2c_msg msgs[] = {
		{
			.addr	= client->addr,	//0x38
			.flags	= 0,	//写
			.len	= 1,	//要写的数据的长度
			.buf	= buf1,
		},
		{
			.addr	= client->addr,
			.flags	= I2C_M_RD,
			.len	= 1,
			.buf	= buf2,
		},
	};
	int ret;
	buf1[0] = addr;
	ret = i2c_transfer(client->adapter, msgs, 2);
	if (ret < 0) {
		pr_err("read reg (0x%02x) error, %d\n", addr, ret);
	} else {
		*pdata = buf2[0];
	}
	return ret;
}
static int i2c_tes_read_fw_reg(struct i2c_client *client,unsigned char *val)
{
	int ret;
	*val = 0xff;
	ret = i2c_tes_read_reg(client,0xa6, val);
	printk("ts reg 0xa6 val is %d\n",*val);
	return ret;
}
static int i2c_open_func(struct file *filp)
{
	return 0;
}

static int i2c_release_func(struct file *filp)
{
	return 0;
}

static ssize_t i2c_read_func(struct file *filp, char __user *buffer, size_t count, loff_t *ppos){
	int ret;
	u8 reg_data;
	
	ret = copy_from_user(&reg_data,buffer,1);
	
	struct i2c_msg msgs[] = {
		{
			.addr	= this_client->addr,	//0x38
			.flags	= 0,	//写
			.len	= 1,	//要写的数据的长度
			.buf	= &reg_data,
		},
		{
			.addr	= this_client->addr,
			.flags	= I2C_M_RD,
			.len	= 1,
			.buf	= &reg_data,
		},
	};
	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret < 0) {
		pr_err("read reg error!\n");
	}
	ret = copy_to_user(buffer,&reg_data,1);
	
	return ret;
}

static ssize_t i2c_write_func(struct file *filp, char __user *buffer, size_t count, loff_t *ppos){
	int ret;
	u8 buf[2];
	struct i2c_msg msgs[1];
	
	ret = copy_from_user(&buf,buffer,2);
	
	msgs[0].addr	= this_client->addr;	//0x38
	msgs[0].flags	= 0;	//写
	msgs[0].len	= 2;	//第一个是要写的寄存器地址，第二个是要写的内容
	msgs[0].buf	= buf;

	ret = i2c_transfer(this_client->adapter, msgs, 1);
	if (ret < 0) {
		pr_err("write reg 0x%02x error!\n",buf[0]);
	}
	ret = copy_to_user(buffer,buf,1);
	
	return ret;
}


static struct file_operations i2c_ops = {
	.owner 	= THIS_MODULE,
	.open 	= i2c_open_func,
	.release= i2c_release_func,
	.write  = i2c_write_func,
	.read 	= i2c_read_func,
};


static struct miscdevice i2c_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.fops	= &i2c_ops,
	.name	= "i2c_control",
};

static int i2c_test_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	unsigned char val;
	printk("==%s:\n", __FUNCTION__);
	
	i2c_tes_read_fw_reg(client,&val);
	
	this_client = client;
	
	misc_register(&i2c_dev);
	
	return 0;
}

static int __devexit i2c_test_remove(struct i2c_client *client)
{
	i2c_set_clientdata(client, NULL);
	misc_deregister(&i2c_dev);
	printk("==%s:\n", __FUNCTION__);
	return 0;
}

static const struct i2c_device_id i2c_test_id[] = {
	{ "i2c_test", 0 },
	{ }
};

static struct i2c_driver i2c_test_driver = {
	.probe		= i2c_test_probe,
	.remove		= __devexit_p(i2c_test_remove),
	.id_table	= i2c_test_id,
	.driver	= {
		.name	= "i2c_test",
		.owner	= THIS_MODULE,
	},
};

static void i2c_io_init(void)
{
	int ret;
	ret = gpio_request(EXYNOS4_GPL0(2), "TP1_EN");
	if (ret) {
		printk(KERN_ERR "failed to request TP1_EN for "
				"I2C control\n");
		//return err;
	}
	gpio_direction_output(EXYNOS4_GPL0(2), 1);
	s3c_gpio_cfgpin(EXYNOS4_GPL0(2), S3C_GPIO_OUTPUT);
	gpio_free(EXYNOS4_GPL0(2));
	mdelay(5);
	
	ret = gpio_request(EXYNOS4_GPX0(3), "GPX0_3");
	if (ret) {
		gpio_free(EXYNOS4_GPX0(3));
		ret = gpio_request(EXYNOS4_GPX0(3), "GPX0_3");
	if(ret)
	{
		printk("ft5xox: Failed to request GPX0_3 \n");
	}
	}
	gpio_direction_output(EXYNOS4_GPX0(3), 0);
	mdelay(200);
	gpio_direction_output(EXYNOS4_GPX0(3), 1);
	s3c_gpio_cfgpin(EXYNOS4_GPX0(3), S3C_GPIO_OUTPUT);
	gpio_free(EXYNOS4_GPX0(3));
	msleep(300);	
}

static int __init i2c_test_init(void)
{
	printk("==%s:\n", __FUNCTION__);
	i2c_io_init();
	printk("==%s:\n", __FUNCTION__);
	return i2c_add_driver(&i2c_test_driver);
}
static void __exit i2c_test_exit(void)
{
	printk("==%s:\n", __FUNCTION__);
	i2c_del_driver(&i2c_test_driver);
}

late_initcall(i2c_test_init);
module_exit(i2c_test_exit);

MODULE_AUTHOR("xunwei_rty");
MODULE_DESCRIPTION("TsI2CTest");
MODULE_LICENSE("GPL");