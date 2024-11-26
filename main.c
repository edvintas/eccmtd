#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#include "eccmtd.h"
#include "eccm.h"

#define TEST
#ifdef TEST
#define TIMEOUT (3000)
#else
#define TIMEOUT (1000*60*5)
#endif

#define BUFFER_SIZE 1024

struct mtd_info *gmtd;
static struct timer_list ecc_timer;

long errorsCount = 0;

void ecc_timer_callback(struct timer_list *timer) {
	pr_info("Timer callback");
	mod_timer(timer, jiffies + msecs_to_jiffies(TIMEOUT));


	char *str = "abcde";
	char stro[32];
	size_t rlen;
	//ecc_mtd_write(gmtd, 0, strlen(str), &rlen, str);
	pr_info("RETLEN = %d", rlen);
//	ecc_mtd_read(gmtd, 0, 10, &rlen, stro);
//	pr_info("STRO = %s", stro);

	int size = 32;
	for (size_t i = 0; i < 1024; i += 10) {
		size_t rlen;
		//ecc_mtd_write(mtd, 0, strlen(str), &rlen, str);
		//pr_info("RETLEN = %d", rlen);
		//ecc_mtd_read(mtd, 0, rlen, &rlen, stro);
		if(ecc_mtd_read(gmtd, i, 10, &rlen, stro))
		{
			ecc_mtd_write(gmtd, i, 10, &rlen, stro);
		}
	}
}

struct mtd_info *get_mtd_info(char *name) {
	struct mtd_info *mtd;
	bool keep_going;
	int n;

	printk(KERN_INFO "Looking for MTD device: %s\n", name);

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
	block obuf[len];
	int ret = mtd_read(mtd, pos, len, retlen, (char*)obuf);
	int rlen;
	if(decode(buf, obuf, len, rlen)) {
		errorsCount++;
	}
	return ret;
}

int ecc_mtd_write(struct mtd_info *mtd, int pos, size_t len, size_t *retlen, char *buf) {
	block ibuf[len];
	int rlen;
	encode(ibuf, buf, len * 8, &rlen);
	int ret = mtd_write(mtd, pos, len * sizeof(block), retlen, (char *)ibuf);
	return ret;
}

static struct proc_dir_entry *ent;

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;

        if (completed) {
                completed = 0;
                return 0;
        }

        completed = 1;

        rv = sprintf(buffer, "Errors detected: %d\n", errorsCount);

        // copies the contents of buffer to userspace usr_buf
        copy_to_user(usr_buf, buffer, rv);

        return rv;
}

static struct proc_ops myops = {
	.proc_read = proc_read
};

static int __init eccmtd_init(void) {
	pr_info("eccm loaded");
	sblkdev_catalog = "eccmtd1,2048;eccmtdX,4096";
	struct mtd_info *mtd = get_mtd_info("mtdram test device");
	if(!mtd) {
		pr_warn("MTD device 'mtdram test device' wan't found, have you executed command?: 'modprobe mtdram total_size=32768'");
		return 0;
	}
	sblkdev_init(mtd);
	gmtd = mtd;

	char *str = "abcde";
	char stro[32];
	size_t rlen;
	ecc_mtd_write(mtd, 0, strlen(str), &rlen, str);
	pr_info("RETLEN = %d", rlen);
	ecc_mtd_read(mtd, 0, rlen, &rlen, stro);
	pr_info("STRO = %s", stro);

	timer_setup(&ecc_timer, ecc_timer_callback, 0);
	mod_timer(&ecc_timer, jiffies + msecs_to_jiffies(TIMEOUT));

	ent=proc_create("eccmtd",0660,NULL,&myops);
	return 0;
}

static void __exit eccmtd_exit(void) {
	pr_info("eccm unloaded");
	del_timer(&ecc_timer);
	sblkdev_exit();
	proc_remove(ent);
}

module_init(eccmtd_init);
module_exit(eccmtd_exit);

MODULE_DESCRIPTION("Error correction module for MTD interface");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edvinas Bazaras <edvintas@gmail.com>");
