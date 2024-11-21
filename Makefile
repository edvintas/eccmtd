NAME:= eccmtd

obj-m += ${NAME}.o device.o sblkdev.o

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
