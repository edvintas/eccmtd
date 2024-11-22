#include <linux/module.h>

#include "eccmtd.h"

static int mtdRW_init(void) {
	struct mtd_info *mtd;
	bool keep_going;
	int n;

	printk(KERN_INFO "mtdRW: Start to unlock MTD partitions!\n");

	keep_going = true;
	for(n = 0; keep_going; n++) {
		mtd = get_mtd_device(NULL, n);
		if(!IS_ERR(mtd)) {
			mtd->flags |= MTD_WRITEABLE;
			printk(KERN_INFO "mtdRW: Make mtd device writeable: %s\n", mtd->name);
			put_mtd_device(mtd);
		} else {
			keep_going = false;
		}
	}

	return 0;
}

static int __init eccmtd_init(void) {
	printk(KERN_INFO "eccm loaded 12344444");
//      sblkdev_add(0, 0, NULL, NULL);
	sblkdev_init();

//      dev = sblkdev_add(sblkdev_major, inx, name, capacity_value);
	return 0;
}

static void __exit eccmtd_exit(void) {
	printk(KERN_INFO "eccm unloaded 45677777777");
	sblkdev_exit();
}

module_init(eccmtd_init);
module_exit(eccmtd_exit);

MODULE_DESCRIPTION("Error correction module for MTD interface");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edvinas Bazaras <edvintas@gmail.com>");
