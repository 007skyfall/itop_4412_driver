#ifndef __BTN_DRV_H__
#define __BTN_DRV_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**************引脚复用寄存器************/
    #define MUX_CTRL_REG_BASE               0x200f0000
    #define GPIO02_7_MUX_CTRL_REG_OFFSET    0x0cc
    #define GPIO01_1_MUX_CTRL_REG_OFFSET    0x0d0
    #define GPIO02_3_MUX_CTRL_REG_OFFSET    0x0bc
    #define GPIO02_4_MUX_CTRL_REG_OFFSET    0x0c0
    #define GPIO11_0_MUX_CTRL_REG_OFFSET    0x0e8
    #define GPIO11_4_MUX_CTRL_REG_OFFSET    0x0f8
    #define GPIO11_7_MUX_CTRL_REG_OFFSET    0x104
    #define GPIO01_2_MUX_CTRL_REG_OFFSET    0x108
    #define GPIO09_2_MUX_CTRL_REG_OFFSET    0x078
    #define GPIO09_4_MUX_CTRL_REG_OFFSET    0x080

/***********GPIO基址********************/
    #define GPIO13_BASE	0x20210000
    #define GPIO12_BASE	0x20200000
    #define GPIO11_BASE	0x201F0000
    #define GPIO10_BASE	0x201E0000
    #define GPIO09_BASE	0x201D0000
    #define GPIO08_BASE	0x201C0000
    #define GPIO07_BASE	0x201B0000
    #define GPIO06_BASE	0x201A0000
    #define GPIO05_BASE	0x20190000
    #define GPIO04_BASE	0x20180000
    #define GPIO03_BASE	0x20170000
    #define GPIO02_BASE	0x20160000
    #define GPIO01_BASE	0x20150000
    #define GPIO00_BASE	0x20140000

/**************GPIO相关寄存器*************/
    #define GPIO_DATA	0x3fc
    #define GPIO_DIR	0x400
    #define GPIO_IS		0x404
    #define GPIO_IBE	0x408
    #define GPIO_IEV	0x40c
    #define GPIO_IE		0x410
    #define GPIO_RIS	0x414
    #define GPIO_MIS	0x418
    #define GPIO_IC		0x41c

/*************GPIO中断号*****************/
    #define IRQ_GPIO0           79
    #define IRQ_GPIO1           80
    #define IRQ_GPIO2_GPIO15    81
    #define IRQ_GPIO3_GPIO14    82
    #define IRQ_GPIO4_GPIO13    83
    #define IRQ_GPIO5_GPIO12    84
    #define IRQ_GPIO6_GPIO11    85
    #define IRQ_GPIO7_GPIO10    86
    #define IRQ_GPIO8_GPIO09    87

    #define IRQ_GPIO02_7    IRQ_GPIO2_GPIO15
    #define IRQ_GPIO01_1    IRQ_GPIO1
    #define IRQ_GPIO02_3    IRQ_GPIO2_GPIO15
    #define IRQ_GPIO02_4    IRQ_GPIO2_GPIO15
    #define IRQ_GPIO11_0    IRQ_GPIO6_GPIO11
    #define IRQ_GPIO11_4    IRQ_GPIO6_GPIO11
    #define IRQ_GPIO11_7    IRQ_GPIO6_GPIO11
    #define IRQ_GPIO01_2    IRQ_GPIO1
    #define IRQ_GPIO09_2    IRQ_GPIO8_GPIO09
    #define IRQ_GPIO09_4    IRQ_GPIO8_GPIO09

    #define GPIO02_7_DATA   (GPIO02_BASE + GPIO_DATA)
    #define GPIO01_1_DATA   (GPIO01_BASE + GPIO_DATA)
    #define GPIO02_3_DATA	(GPIO02_BASE + GPIO_DATA)
    #define GPIO02_4_DATA	(GPIO02_BASE + GPIO_DATA)
    #define GPIO11_0_DATA   (GPIO11_BASE + GPIO_DATA)
    #define GPIO11_4_DATA   (GPIO11_BASE + GPIO_DATA)
    #define GPIO11_7_DATA   (GPIO11_BASE + GPIO_DATA)
    #define GPIO01_2_DATA   (GPIO01_BASE + GPIO_DATA)
    #define GPIO09_2_DATA   (GPIO09_BASE + GPIO_DATA)
    #define GPIO09_4_DATA   (GPIO09_BASE + GPIO_DATA)

    //读写写寄存器
    #define  GPIO_WRITE_REG(base_addr,offset_addr, value) (*(volatile unsigned int *)((unsigned int)ioremap_nocache(base_addr,0x800) + offset_addr) = (value))
    #define  GPIO_READ_REG(base_addr,offset_addr)         (*(volatile unsigned int *)((unsigned int)ioremap_nocache(base_addr,0x800) + offset_addr))


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif	/* __BTN_DRV_H__ */
