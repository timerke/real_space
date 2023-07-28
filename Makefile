export ARCH:=arm
#export CROSS_COMPILE:=arm-xilinx-linux-gnueabi-
export CROSS_COMPILE:=arm-linux-gnueabihf-

CC=$(CROSS_COMPILE)gcc
OBJDUMP=$(CROSS_COMPILE)objdump



KDIR:= /home/user/buildroot-2016.08.1/output/build/linux-xilinx-v2016.2
PWD := $(shell pwd)

obj-m := xdma.o


default:
	${MAKE} -C ${KDIR} M=${PWD} modules
	$(OBJDUMP) -S -d -t -h xdma.ko > xdma.objdump

clean:
	${MAKE} -C ${KDIR} M=${PWD} clean
	rm -rf *.lst *.objdump