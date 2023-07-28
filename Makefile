export ARCH:=arm
#export CROSS_COMPILE:=arm-xilinx-linux-gnueabi-
#export CROSS_COMPILE:=arm-linux-gnueabihf-

CC=$(CROSS_COMPILE)gcc
OBJDUMP=$(CROSS_COMPILE)objdump

KERNEL_DIR:= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m := xdma.o

default:
	${MAKE} -C ${KERNEL_DIR} M=${PWD} modules
	$(OBJDUMP) -S -d -t -h xdma.ko > xdma.objdump

clean:
	${MAKE} -C ${KERNEL_DIR} M=${PWD} clean
	rm -rf *.lst *.objdump