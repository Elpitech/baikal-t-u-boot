/* SPDX-License-Identifier: GPL-2.0+ */

/*
 *
 */

#ifndef __SMIFB_H__
#define __SMIFB_H__

#include <common.h>

struct smifb_priv {
	void *fb_start;
	unsigned long mem_size;
	void __iomem *regs;
	struct sm750_dev sm750_dev;
};

int smi_move_rect(struct udevice *dev, uint dx, uint dy, uint sx, uint sy,
		  uint w, uint h);
int smi_fill_rect(struct udevice *dev, uint dx, uint dy, uint w, uint h,
		  int clr);
int smi_set_cursor(struct udevice *dev, int x, int y);

#endif /* __SMIFB_H__ */
