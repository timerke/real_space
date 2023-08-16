export ARCH=arm
#export CROSS_COMPILE=arm-xilinx-linux-gnueabi-
export CROSS_COMPILE=arm-linux-gnueabihf-

CC=$(CROSS_COMPILE)gcc
OBJDUMP=$(CROSS_COMPILE)objdump
#KERNEL_DIR=/lib/modules/$(shell uname -r)/build
KERNEL_DIR=/home/user/buildroot-2016.08.1/output/build/linux-xilinx-v2016.2
PWD=$(shell pwd)
obj-m=xdma.o

default:
	${MAKE} -C ${KERNEL_DIR} M=${PWD} modules
	$(OBJDUMP) -S -d -t -h xdma.ko > xdma.objdump

hello: hello.c
	${MAKE} -C ${KERNEL_DIR} M=${PWD} modules

example.o: example.c xdma.h
	$(CC) -c -o example.o example.c

example: example.o xdma.o
	$(CC) -o example example.o xdma.o

clean:
	${MAKE} -C ${KERNEL_DIR} M=${PWD} clean
	rm -rf *.lst *.objdump hello