#ifndef __SBLKDEV_H__
#define __SBLKDEV_H__

extern char *sblkdev_catalog;

int sblkdev_init(void *mtd);
void sblkdev_exit(void);

#endif
