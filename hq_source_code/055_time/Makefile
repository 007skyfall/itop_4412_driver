#!/bin/bash
$(warning KERNELRELEASE = $(KERNELRELEASE))

ifeq ($(KERNELRELEASE),)

#内核的源码路径, ?= 条件赋值, uname -r 得到内核版本号
#KERNELDIR ?=  /home/skyfall/pi/linux-rpi-4.14.y
#KERNELDIR ?=  /home/skyfall/iTOP4412/kernel/iTop4412_Kernel_3.0
KERNELDIR ?=  /home/skyfall/week_bak/linux-3.18.132

# := 立即赋值, 得到当前的绝对路径
PWD := $(shell pwd)


# -C 切换工作路径, $(MAKE) =  make
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module* modules*

.PHONY: modules clean

else
	  # 生成模块
       obj-m := time.o 
	
endif

