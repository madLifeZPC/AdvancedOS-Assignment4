OBJ = fourmb_device
obj-m := $(OBJ).o
modules-objs:= fourmb_device.o

KERNELDIR = /lib/modules/`uname -r`/build
PWD = $(shell pwd)
  
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	insmod $(OBJ).ko

uninstall:
	rmmod $(OBJ).ko

  
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
