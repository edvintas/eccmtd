#include <linux/module.h>

#include "eccmtd.h"

int mtdRW_init(void) {
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

struct mtd_info *get_mtd_info(char *name) {
	struct mtd_info *mtd;
	bool keep_going;
	int n;

	printk(KERN_INFO "mtdRW: Start to unlock MTD partitions!\n");

	keep_going = true;
	for(n = 0; keep_going; n++) {
		mtd = get_mtd_device(NULL, n);
		if(!IS_ERR(mtd)) {
			pr_info("MTD name = %s", mtd->name);
			if(!strcmp(mtd->name, name)) {
				pr_info(" Mtd device found: %s\n", mtd->name);
				return mtd;
			}
		} else {
			keep_going = false;
		}
	}

	return 0;
}


int ecc_mtd_read(struct mtd_info *mtd, int pos, size_t len, size_t *retlen, char *buf) {
	block *obuf = kzalloc(len, GFP_KERNEL);
	int ret = mtd_read(mtd, pos, len, retlen, (char*)obuf);
	int rlen;
	decode(buf, obuf, len, rlen);
	kfree(obuf);
	return ret;
}

int ecc_mtd_write(struct mtd_info *mtd, int pos, size_t len, size_t *retlen, char *buf) {
	block *ibuf = kzalloc(len, GFP_KERNEL);
	int rlen;
	encode(ibuf, buf, len, &rlen);
//	memcpy(ibuf, buf, len);
	int ret = mtd_write(mtd, pos, len * sizeof(block), retlen, (char *)ibuf);
	kfree(ibuf);
	return ret;
}

static int __init eccmtd_init(void) {
	pr_info("eccm loaded 12344444");
//      sblkdev_add(0, 0, NULL, NULL);
	sblkdev_init();
	struct mtd_info *mtd = get_mtd_info("mtdram test device");
	char *str = "abcdefghijklm";
	char stro[32];
	size_t rlen;
	ecc_mtd_write(mtd, 0, strlen(str), &rlen, str);
	pr_info("RETLEN = %d", rlen);
	ecc_mtd_read(mtd, 0, rlen, &rlen, stro);
	pr_info("STRO = %s", stro);
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
