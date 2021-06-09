ARCH:=arm64

ifeq ($(ARCH), )
$(error make ARCH=arm/arm64)
endif


ifeq ($(ARCH),arm64)

ifneq ($(KERNELRELEASE),)
obj-m:=hello.o
else
KERNELDIR?=/home/song/workspace/repo/fy12-repo/osdrv/opensource/kernel/linux-4.9.138/
PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- M=$(PWD) modules
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.order  *.symvers
.PHONY: modules modules_install clean
endif

else


ifeq ($(ARCH),arm)

ifneq ($(KERNELRELEASE),)
obj-m:=hello.o
else
KERNELDIR?=/home/song/workspace/repo/fy12-cp/osdrv/opensource/kernel/linux-4.9.138/
PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=arm-mol-linux-uclibcgnueabihf- M=$(PWD) modules
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c *.order  *.symvers
.PHONY: modules modules_install clean
endif

else
$(error ARCH=arm/arm64 error)
endif

endif
