NAME:= eccmtd

-include ${M}/Makefile.sblkdev

${NAME}-y := main.o device.o hamming.o sblkdev.o
obj-$(CONFIG_SBLKDEV) += ${NAME}.o


build:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

sign:
	kmodsign sha512 MOK.priv MOK.der ${NAME}.ko

load:
	insmod ${NAME}.ko

unload:
	rmmod ${NAME}.ko

format:
	astyle --options=.astylerc -r '*.cpp,*.c,*.hpp,*.h,*.c'

log:
	dmesg
