#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "sblkdev.h"

static int eccmtd_init(void) {
	printk(KERN_INFO "eccmtd loaded 123");
//	dev = sblkdev_add(sblkdev_major, inx, name, capacity_value);
	return 0;
}

static void __exit eccmtd_exit(void) {
	printk(KERN_INFO "eccmtd unloaded 456");
}

module_init(eccmtd_init);
module_exit(eccmtd_exit);

MODULE_DESCRIPTION("Error correction module for MTD interface");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edvinas Bazaras <edvintas@gmail.com>");
