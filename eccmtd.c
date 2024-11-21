#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "sblkdev.h"

static int eccmtd_init(void) {
	printk(KERN_INFO "eccm loaded 12344444");
	sblkdev_add(0, 0, NULL, NULL);
//	sblkdev_init();
//	dev = sblkdev_add(sblkdev_major, inx, name, capacity_value);
	return 0;
}

static void __exit eccmtd_exit(void) {
	printk(KERN_INFO "eccm unloaded 45677777777");
//	sblkdev_exit();
}

module_init(eccmtd_init);
module_exit(eccmtd_exit);

MODULE_DESCRIPTION("Error correction module for MTD interface");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edvinas Bazaras <edvintas@gmail.com>");
