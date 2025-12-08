KERNEL_DIR ?= /usr/src/kernel-6.16.1-eandres

obj-m := hola.o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
