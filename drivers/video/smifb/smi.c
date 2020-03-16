// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2016, Bin Meng <bmeng.cn@gmail.com>
 */

#include <common.h>
#include <dm.h>
#include <pci.h>
#include <video.h>
#include <asm/io.h>
#include "sm750.h"
#include "smifb.h"
#include "sm750_accel.h"
#include "ddk750_chip.h"
#include <dm/devres.h>

#define PCI_VENDOR_ID_SMI	0x126f
#define PCI_CHIP_SM750		0x750
#define PCI_CHIP_SM768		0x768

int smi_move_rect(struct udevice *dev, uint dx, uint dy, uint sx, uint sy,
		  uint w, uint h)
{
	struct smifb_priv *priv = dev_get_priv(dev);
	struct video_priv *uc_priv = dev_get_uclass_priv(dev);
	struct sm750_dev *sm750_dev = &priv->sm750_dev;
	struct lynx_accel *accel = &sm750_dev->accel;
	int rc;

	if (!accel->de_copyarea) {
		eprintf("smi_move_rect: de_copyarea not set\n");
		return -ENOSYS;
	}

	rc = accel->de_copyarea(accel, 0, uc_priv->line_length, sx, sy,
				0, uc_priv->line_length,
				(1 << uc_priv->bpix) >> 3,
				dx, dy, w, h, HW_ROP2_COPY);

	return rc;
}

int smi_fill_rect(struct udevice *dev, uint dx, uint dy, uint w, uint h,
		  int clr)
{
	struct smifb_priv *priv = dev_get_priv(dev);
	struct video_priv *uc_priv = dev_get_uclass_priv(dev);
	struct sm750_dev *sm750_dev = &priv->sm750_dev;
	struct lynx_accel *accel = &sm750_dev->accel;
	int rc;

	if (!accel->de_fillrect) {
		eprintf("smi_fill_rect: de_fillrect not set\n");
		return -ENOSYS;
	}

	rc = accel->de_fillrect(accel, 0, uc_priv->line_length,
				(1 << uc_priv->bpix) >> 3,
				dx, dy, w, h, clr, HW_ROP2_COPY);

	return rc;
}

static int smi_video_probe(struct udevice *dev)
{
	struct smifb_priv *priv = dev_get_priv(dev);
	struct pci_child_plat *plat = dev_get_parent_plat(dev);
	struct video_uc_plat *uc_plat = dev_get_uclass_plat(dev);
	struct video_priv *uc_priv = dev_get_uclass_priv(dev);
	struct sm750_dev *sm750_dev = &priv->sm750_dev;
	void * __iomem addr;
	struct fb_info *info;
	struct lynxfb_par *par;
	int rc = -ENXIO;
	char *modeparam;

	addr = dm_pci_map_bar(dev, 0x10, 0);
	priv->fb_start = addr;
	addr = dm_pci_map_bar(dev, 0x14, 0);
	priv->regs = addr;

	sm750_dev->vidreg_start = (unsigned long)priv->regs;
	sm750_dev->pvReg = priv->regs;
	sm750_dev->devid = plat->device;
	sm750_dev->revid = 0xa1;
	sm750_dev->vidmem_start = (unsigned long)priv->fb_start;
	sm750_dev->pvMem = priv->fb_start;

	if (plat->device == PCI_CHIP_SM750) {
		modeparam = env_get("videomode");
		if (modeparam)
			sm750fb_setup(sm750_dev, modeparam);
		ddk750_set_mmio(sm750_dev->pvReg, sm750_dev->devid,
				sm750_dev->revid);
		rc = hw_sm750_inithw(sm750_dev);
	}

	if (!rc) {
		info = devm_kzalloc(dev, sizeof(struct fb_info), 0);
		if (!info) {
			printf("No memory for fb_info\n");
			return -ENOMEM;
		}
		info->par = devm_kzalloc(dev, sizeof(struct lynxfb_par), 0);
		if (!info->par) {
			printf("No memory for fb_par\n");
			return -ENOMEM;
		}
		sm750_dev->fbinfo[0] = info;
		par = info->par;
		par->dev = sm750_dev;

		rc = lynxfb_set_fbinfo(info, 0);
		if (rc) {
			printf("set_fbinfo failed (%d)\n", rc);
			return rc;
		}

		rc = lynxfb_ops_set_par(info);
		if (rc) {
			printf("set_par failed (%d)\n", rc);
			return rc;
		}

		uc_priv->xsize = info->var.xres;
		uc_priv->ysize = info->var.yres;
		switch (info->var.bits_per_pixel) {
		case 32:
		case 24:
			uc_priv->bpix = VIDEO_BPP32;
			uc_priv->line_length = uc_priv->xsize * 4;
			break;
		case 16:
			uc_priv->bpix = VIDEO_BPP16;
			uc_priv->line_length = uc_priv->xsize * 2;
			break;
		case 8:
			uc_priv->bpix = VIDEO_BPP8;
			uc_priv->line_length = uc_priv->xsize;
			break;
		default:
			return -EPROTONOSUPPORT;
		}
		uc_priv->vidconsole_drv_name = "smi_vidconsole";
		uc_plat->base = (unsigned long)priv->fb_start;
		uc_plat->size = uc_priv->line_length * uc_priv->ysize;

		printf("Video: %dx%dx%d\n", uc_priv->xsize, uc_priv->ysize,
		       info->var.bits_per_pixel);

		rc = hw_sm750_output_setMode(&par->output, &info->var,
					     &info->fix);
		printf("setMode: %d\n", rc);
	}
	return rc;
}

static const struct udevice_id smi_video_ids[] = {
	{ .compatible = "smi-fb" },
	{ }
};

U_BOOT_DRIVER(smi_video) = {
	.name	= "smi_video",
	.id	= UCLASS_VIDEO,
	.of_match = smi_video_ids,
	.probe	= smi_video_probe,
	.priv_auto = sizeof(struct smifb_priv),
};

static struct pci_device_id smi_video_supported[] = {
	{PCI_DEVICE(PCI_VENDOR_ID_SMI, PCI_CHIP_SM750)},
	{PCI_DEVICE(PCI_VENDOR_ID_SMI, PCI_CHIP_SM768)},
	{ },
};

U_BOOT_PCI_DEVICE(smi_video, smi_video_supported);
