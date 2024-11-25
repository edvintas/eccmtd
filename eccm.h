#ifndef __ECCM_H__
#define __ECCM_H__

#include <linux/mtd/mtd.h>
#include <linux/module.h>

int ecc_mtd_read(struct mtd_info *mtd, int pos, size_t len, size_t *retlen, char *buf);
int ecc_mtd_write(struct mtd_info *mtd, int pos, size_t len, size_t *retlen, char *buf);

#endif
