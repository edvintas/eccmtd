NAME:= eccmtd

-include ${M}/Makefile.sblkdev

${NAME}-y := main.o device.o hamming.o sblkdev.o
obj-$(CONFIG_SBLKDEV) += ${NAME}.o

all: build sign
	@echo All done.

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

create-mtdram:
	modprobe mtdram total_size=32768
	cat /proc/mtd

build-hamming-test:
	gcc test_hamming.c hamming.c -lm -o test_hamming

test-hamming:
	./test_hamming

test: build-hamming-test test-hamming
	@echo Tests done.
