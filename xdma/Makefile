export ARCH:=arm
export CROSS_COMPILE:=arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
OBJDUMB = $(CROSS_COMPILE)objdump
KERNEL_DIR = /home/user/buildroot-2016.08.1/output/build/linux-xilinx-v2016.2
PWD = $(shell pwd)
obj-m = my_dma.o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
	$(OBJDUMP) -S -d -t -h my_dma.ko > xdma.objdump

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
	rm -rf *.lst *.objdump