KERNEL_VERSION = $(shell uname -r)
PWD = $(shell pwd)
obj-m = hello.o

all:
	make -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) clean

