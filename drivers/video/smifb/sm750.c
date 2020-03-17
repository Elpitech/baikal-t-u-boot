// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <linux/ctype.h>
#include <linux/fb.h>
#include <asm/io.h>
#include "sm750.h"
#include "sm750.h"
#include "sm750_accel.h"
#include "sm750_cursor.h"

#include "modedb.h"

/* common var for all device */
static const char *g_fbmode[] = {NULL, NULL};
static const struct fb_videomode *g_def_fbmode = &modedb2[17];

static const struct fb_videomode lynx750_ext[] = {
	/*      1024x600-60 VESA        [1.71:1] */
	{NULL,  60, 1024, 600, 20423, 144,  40, 18, 1, 104, 3,
	 FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	 FB_VMODE_NONINTERLACED},

	/*      1024x600-70 VESA */
	{NULL,  70, 1024, 600, 17211, 152,  48, 21, 1, 104, 3,
	 FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	 FB_VMODE_NONINTERLACED},

	/*      1024x600-75 VESA */
	{NULL,  75, 1024, 600, 15822, 160,  56, 23, 1, 104, 3,
	 FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	 FB_VMODE_NONINTERLACED},

	/*      1024x600-85 VESA */
	{NULL,  85, 1024, 600, 13730, 168,  56, 26, 1, 112, 3,
	 FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	 FB_VMODE_NONINTERLACED},

	/*      720x480 */
	{NULL, 60,  720,  480,  37427, 88,   16, 13, 1,   72,  3,
	 FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	 FB_VMODE_NONINTERLACED},

	/*      1280x720                [1.78:1]        */
	{NULL, 60,  1280,  720,  13426, 162, 86, 22, 1,  136, 3,
	 FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	 FB_VMODE_NONINTERLACED},

	/*      1280x768@60 */
	{NULL, 60, 1280, 768, 12579, 192, 64, 20, 3, 128, 7,
	 FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	 FB_VMODE_NONINTERLACED},

	/*      1360 x 768      [1.77083:1]     */
	{NULL,  60, 1360, 768, 11804, 208,  64, 23, 1, 144, 3,
	 FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	 FB_VMODE_NONINTERLACED},

	/*      1368 x 768      [1.78:1]        */
	{NULL, 60,  1368,  768,  11647, 216, 72, 23, 1,  144, 3,
	 FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	 FB_VMODE_NONINTERLACED},

	/*      1440 x 900              [16:10] */
	{NULL, 60, 1440, 900, 9392, 232, 80, 28, 1, 152, 3,
	 FB_SYNC_VERT_HIGH_ACT,
	 FB_VMODE_NONINTERLACED},

	/*      1440x960                [15:10] */
	{NULL, 60, 1440, 960, 8733, 240, 88, 30, 1, 152, 3,
	 FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	 FB_VMODE_NONINTERLACED},

	/*      1920x1080       [16:9]  */
	{NULL, 60, 1920, 1080, 6734, 148, 88, 41, 1, 44, 3,
	 FB_SYNC_VERT_HIGH_ACT,
	 FB_VMODE_NONINTERLACED},
};

int lynxfb_ops_set_par(struct fb_info *info)
{
	struct lynxfb_par *par;
	struct lynxfb_crtc *crtc;
	struct fb_var_screeninfo *var;
	struct fb_fix_screeninfo *fix;
	int ret;
	unsigned int line_length;

	if (!info)
		return -EINVAL;

	ret = 0;
	par = info->par;
	crtc = &par->crtc;
	var = &info->var;
	fix = &info->fix;

	/* fix structure is not so FIX ... */
	line_length = var->xres_virtual * var->bits_per_pixel / 8;
	line_length = ALIGN(line_length, crtc->line_pad);
	fix->line_length = line_length;
	pr_info("fix->line_length = %d\n", fix->line_length);

	/*
	 * var->red,green,blue,transp are need to be set by driver
	 * and these data should be set before setcolreg routine
	 */

	switch (var->bits_per_pixel) {
	case 8:
		fix->visual = FB_VISUAL_PSEUDOCOLOR;
		var->red.offset = 0;
		var->red.length = 8;
		var->green.offset = 0;
		var->green.length = 8;
		var->blue.offset = 0;
		var->blue.length = 8;
		var->transp.length = 0;
		var->transp.offset = 0;
		break;
	case 16:
		var->red.offset = 11;
		var->red.length = 5;
		var->green.offset = 5;
		var->green.length = 6;
		var->blue.offset = 0;
		var->blue.length = 5;
		var->transp.length = 0;
		var->transp.offset = 0;
		fix->visual = FB_VISUAL_TRUECOLOR;
		break;
	case 24:
	case 32:
		var->red.offset = 16;
		var->red.length = 8;
		var->green.offset = 8;
		var->green.length = 8;
		var->blue.offset = 0;
		var->blue.length = 8;
		fix->visual = FB_VISUAL_TRUECOLOR;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	var->height = -1;
	var->width = -1;
	var->accel_flags = 0;

	if (ret) {
		pr_err("pixel bpp format not satisfied\n.");
		return ret;
	}
	return hw_sm750_crtc_setMode(crtc, var, fix);
}

static int lynxfb_ops_check_var(struct fb_var_screeninfo *var,
				struct fb_info *info)
{
	struct lynxfb_par *par;
	struct lynxfb_crtc *crtc;
	unsigned long request;

	par = info->par;
	crtc = &par->crtc;

	pr_debug("check var:%dx%d-%d\n",
		 var->xres,
		 var->yres,
		 var->bits_per_pixel);

	switch (var->bits_per_pixel) {
	case 8:
		info->fix.visual = FB_VISUAL_PSEUDOCOLOR;
		var->red.offset = 0;
		var->red.length = 8;
		var->green.offset = 0;
		var->green.length = 8;
		var->blue.offset = 0;
		var->blue.length = 8;
		var->transp.length = 0;
		var->transp.offset = 0;
		break;
	case 16:
		var->red.offset = 11;
		var->red.length = 5;
		var->green.offset = 5;
		var->green.length = 6;
		var->blue.offset = 0;
		var->blue.length = 5;
		var->transp.length = 0;
		var->transp.offset = 0;
		info->fix.visual = FB_VISUAL_TRUECOLOR;
		break;
	case 24:
	case 32:
		var->red.offset = 16;
		var->red.length = 8;
		var->green.offset = 8;
		var->green.length = 8;
		var->blue.offset = 0;
		var->blue.length = 8;
		info->fix.visual = FB_VISUAL_TRUECOLOR;
		break;
	default:
		pr_err("bpp %d not supported\n", var->bits_per_pixel);
		return -EINVAL;
	}
	var->height = -1;
	var->width = -1;
	var->accel_flags = 0;

	/* check if current fb's video memory big enought to hold the
	 * onscreen
	 */
	request = var->xres_virtual * (var->bits_per_pixel >> 3);
	/* defaulty crtc->channel go with par->index */

	request = ALIGN(request, crtc->line_pad);
	request = request * var->yres_virtual;
	if (crtc->vidmem_size < request) {
		pr_err("not enough video memory for mode\n");
		return -ENOMEM;
	}

	return hw_sm750_crtc_checkMode(crtc, var);
}

static int sm750fb_set_drv(struct lynxfb_par *par)
{
	int ret;
	struct sm750_dev *sm750_dev;
	struct lynxfb_output *output;
	struct lynxfb_crtc *crtc;

	ret = 0;

	sm750_dev = par->dev;
	output = &par->output;
	crtc = &par->crtc;

	crtc->vidmem_size = sm750_dev->vidmem_size;

	crtc->line_pad = 16;
	crtc->xpanstep = 8;
	crtc->ypanstep = 1;
	crtc->ywrapstep = 0;

	/* chip specific phase */
	sm750_dev->accel.de_wait = (sm750_dev->revid == SM750LE_REVISION_ID) ?
					hw_sm750le_deWait :
					hw_sm750_deWait;
	switch (sm750_dev->dataflow) {
	case sm750_simul_pri:
		output->paths = sm750_pnc;
		crtc->channel = sm750_primary;
		crtc->oScreen = 0;
		crtc->vScreen = sm750_dev->pvMem;
		pr_info("use simul primary mode\n");
		break;
	case sm750_simul_sec:
		output->paths = sm750_pnc;
		crtc->channel = sm750_secondary;
		crtc->oScreen = 0;
		crtc->vScreen = sm750_dev->pvMem;
		break;
	case sm750_dual_normal:
		if (par->index == 0) {
			output->paths = sm750_panel;
			crtc->channel = sm750_primary;
			crtc->oScreen = 0;
			crtc->vScreen = sm750_dev->pvMem;
		} else {
			output->paths = sm750_crt;
			crtc->channel = sm750_secondary;
			/* not consider of padding stuffs for
			 * oScreen,need fix
			 */
			crtc->oScreen = (sm750_dev->vidmem_size >> 1);
			crtc->vScreen = sm750_dev->pvMem + crtc->oScreen;
		}
		break;
	case sm750_dual_swap:
		if (par->index == 0) {
			output->paths = sm750_panel;
			crtc->channel = sm750_secondary;
			crtc->oScreen = 0;
			crtc->vScreen = sm750_dev->pvMem;
		} else {
			output->paths = sm750_crt;
			crtc->channel = sm750_primary;
			/* not consider of padding stuffs for
			 * oScreen,need fix
			 */
			crtc->oScreen = (sm750_dev->vidmem_size >> 1);
			crtc->vScreen = sm750_dev->pvMem + crtc->oScreen;
		}
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}

static int lynxfb_parse_video_mode(const char *s, unsigned int *xres,
				   unsigned int *yres, unsigned int *freq)
{
	/* Get the X-resolution*/
	while (*s && !isdigit(*s))
		s++;
	*xres = simple_strtoul(s, (char **)&s, 10);
	if (!*xres)
		return 0;

	/* Get the Y-resolution */
	while (*s && !isdigit(*s))
		s++;
	*yres = simple_strtoul(s, (char **)&s, 10);
	if (!*yres)
		return 0;

	/* Get the frequency */
	while (*s && !isdigit(*s))
		s++;
	*freq = simple_strtoul(s, (char **)&s, 10);
	if (!*freq)
		return 0;

	return 1;
}

static const struct fb_videomode *
lynxfb_find_video_mode(const struct fb_videomode *modes,
		       unsigned int mode_cnt, unsigned int xres,
		       unsigned int yres, unsigned int freq)
{
	unsigned int i;

	for (i = 0; i < mode_cnt; ++i)
		if (modes[i].xres == xres &&
		    modes[i].yres == yres &&
		    modes[i].refresh == freq)
			return &modes[i];

	return NULL;
}

static const struct fb_videomode *lynxfb_get_video_mode(const char *s)
{
	unsigned int xres, yres, freq;
	const struct fb_videomode *mode;

	if (!lynxfb_parse_video_mode(s, &xres, &yres, &freq))
		return NULL;

	mode = lynxfb_find_video_mode(lynx750_ext, ARRAY_SIZE(lynx750_ext),
				      xres, yres, freq);
	if (mode)
		return mode;

	mode = lynxfb_find_video_mode(modedb2, ARRAY_SIZE(modedb2),
				      xres, yres, freq);
	if (mode)
		return mode;

	return NULL;
}

int lynxfb_set_fbinfo(struct fb_info *info, int index)
{
	struct lynxfb_par *par;
	struct sm750_dev *sm750_dev;
	struct lynxfb_crtc *crtc;
	struct lynxfb_output *output;
	struct fb_var_screeninfo *var;
	struct fb_fix_screeninfo *fix;
	const struct fb_videomode *mode;

	static const char *fixId[2] = {
		"sm750_fb1", "sm750_fb2",
	};

	int ret, line_length;

	ret = 0;
	par = (struct lynxfb_par *)info->par;
	sm750_dev = par->dev;
	crtc = &par->crtc;
	output = &par->output;
	var = &info->var;
	fix = &info->fix;

	/* set index */
	par->index = index;
	output->channel = &crtc->channel;
	sm750fb_set_drv(par);

	/*
	 * set current cursor variable and proc pointer,
	 * must be set after crtc member initialized
	 */
	crtc->cursor.offset = crtc->oScreen + crtc->vidmem_size - 1024;
	crtc->cursor.mmio = sm750_dev->pvReg + 0x800f0 +
				(int)crtc->channel * 0x140;

	pr_info("crtc->cursor.mmio = %p\n", crtc->cursor.mmio);
	crtc->cursor.maxH = 64;
	crtc->cursor.maxW = 64;
	crtc->cursor.size = crtc->cursor.maxH * crtc->cursor.maxW * 2 / 8;
	crtc->cursor.vstart = (char *)sm750_dev->pvMem + crtc->cursor.offset;
	memset_io(crtc->cursor.vstart, 0, crtc->cursor.size);

	/* Use color #1 for cursor, set lines 12-13 */
	*(u32 *)(crtc->cursor.vstart + 16 * 12) = 0x5555;
	*(u32 *)(crtc->cursor.vstart + 16 * 13) = 0x5555;
#ifdef CONFIG_SYS_WHITE_ON_BLACK
	hw_cursor_setColor(&crtc->cursor, 0xc318, 0); /* light grey */
#else
	hw_cursor_setColor(&crtc->cursor, 0, 0); /* black */
#endif
	hw_cursor_setPos(&crtc->cursor, 0, 0);
	hw_cursor_enable(&crtc->cursor);

	if (!g_fbmode[index]) {
		mode = g_def_fbmode;
	} else {
		mode = lynxfb_get_video_mode(g_fbmode[index]);
		if (!mode)
			mode = g_def_fbmode;
	}

	var->xres = mode->xres;
	var->yres = mode->yres;
	var->xres_virtual = mode->xres;
	var->yres_virtual = mode->yres;
	var->xoffset = 0;
	var->yoffset = 0;
	var->bits_per_pixel = 32;
	var->pixclock = mode->pixclock;
	var->left_margin = mode->left_margin;
	var->right_margin = mode->right_margin;
	var->upper_margin = mode->upper_margin;
	var->lower_margin = mode->lower_margin;
	var->hsync_len = mode->hsync_len;
	var->vsync_len = mode->vsync_len;
	var->sync = mode->sync;
	var->vmode = mode->vmode;
	fix->line_length = var->xres * var->bits_per_pixel / 8;

	/* some member of info->var had been set by fb_find_mode */

	pr_info("Member of info->var is :\n"
		"    xres=%d\n"
		"    yres=%d\n"
		"    xres_virtual=%d\n"
		"    yres_virtual=%d\n"
		"    xoffset=%d\n"
		"    yoffset=%d\n"
		"    bits_per_pixel=%d\n"
		"    ...\n",
		var->xres,
		var->yres,
		var->xres_virtual,
		var->yres_virtual,
		var->xoffset,
		var->yoffset,
		var->bits_per_pixel);

	/* set par */
	par->info = info;

	/* set info */
	line_length = ALIGN((var->xres_virtual * var->bits_per_pixel / 8),
			    crtc->line_pad);

	info->pseudo_palette = &par->pseudo_palette[0];
	info->screen_base = (char *)crtc->vScreen;
	pr_debug("screen_base vaddr = %p\n", info->screen_base);
	info->screen_size = line_length * var->yres_virtual;
	info->flags = FBINFO_FLAG_DEFAULT | 0;

	/* set info->fix */
	fix->type = FB_TYPE_PACKED_PIXELS;
	fix->type_aux = 0;
	fix->xpanstep = crtc->xpanstep;
	fix->ypanstep = crtc->ypanstep;
	fix->ywrapstep = crtc->ywrapstep;
	fix->accel = FB_ACCEL_SMI;

	strlcpy(fix->id, fixId[index], sizeof(fix->id));

	fix->smem_start = crtc->oScreen + sm750_dev->vidmem_start;
	pr_info("fix->smem_start = %lx\n", fix->smem_start);
	/*
	 * according to mmap experiment from user space application,
	 * fix->mmio_len should not larger than virtual size
	 * (xres_virtual x yres_virtual x ByPP)
	 * Below line maybe buggy when user mmap fb dev node and write
	 * data into the bound over virtual size
	 */
	fix->smem_len = crtc->vidmem_size;
	pr_info("fix->smem_len = %x\n", fix->smem_len);
	fix->line_length = line_length;
	fix->mmio_start = sm750_dev->vidreg_start;
	pr_info("fix->mmio_start = %lx\n", fix->mmio_start);
	fix->mmio_len = sm750_dev->vidreg_size;
	pr_info("fix->mmio_len = %x\n", fix->mmio_len);
	switch (var->bits_per_pixel) {
	case 8:
		fix->visual = FB_VISUAL_PSEUDOCOLOR;
		break;
	case 16:
	case 32:
		fix->visual = FB_VISUAL_TRUECOLOR;
		break;
	}

	/* set var */
	var->activate = FB_ACTIVATE_NOW;
	var->accel_flags = 0;
	var->vmode = FB_VMODE_NONINTERLACED;

	lynxfb_ops_check_var(var, info);
	return ret;
}

/* chip specific g_option configuration routine */
void sm750fb_setup(struct sm750_dev *sm750_dev, char *src)
{
	char *opt;
	int swap;

	swap = 0;

	sm750_dev->initParm.chip_clk = 0;
	sm750_dev->initParm.mem_clk = 0;
	sm750_dev->initParm.master_clk = 0;
	sm750_dev->initParm.powerMode = 0;
	sm750_dev->initParm.setAllEngOff = 0;
	sm750_dev->initParm.resetMemory = 1;

	if (!src || !*src) {
		pr_warn("sm750: no specific g_option.\n");
		goto NO_PARAM;
	}

	while ((opt = strsep(&src, ":")) != NULL && *opt != 0) {
		pr_info("opt=%s\n", opt);
		pr_info("src=%s\n", src);

		if (!strncmp(opt, "swap", strlen("swap"))) {
			swap = 1;
		} else if (!strncmp(opt, "nocrt", strlen("nocrt"))) {
			sm750_dev->nocrt = 1;
		} else if (!strncmp(opt, "36bit", strlen("36bit"))) {
			sm750_dev->pnltype = sm750_doubleTFT;
		} else if (!strncmp(opt, "18bit", strlen("18bit"))) {
			sm750_dev->pnltype = sm750_dualTFT;
		} else if (!strncmp(opt, "24bit", strlen("24bit"))) {
			sm750_dev->pnltype = sm750_24TFT;
		} else {
			if (!g_fbmode[0]) {
				g_fbmode[0] = opt;
				pr_info("find fbmode0 : %s\n", g_fbmode[0]);
			} else if (!g_fbmode[1]) {
				g_fbmode[1] = opt;
				pr_info("find fbmode1 : %s\n", g_fbmode[1]);
			} else {
				pr_warn("How many view you wann set?\n");
			}
		}
	}

NO_PARAM:
	if (sm750_dev->revid != SM750LE_REVISION_ID) {
		if (swap)
			sm750_dev->dataflow = sm750_simul_sec;
		else
			sm750_dev->dataflow = sm750_simul_pri;
	} else {
		/* SM750LE only have one crt channel */
		sm750_dev->dataflow = sm750_simul_sec;
		/* sm750le do not have complex attributes */
		sm750_dev->nocrt = 0;
	}
}

