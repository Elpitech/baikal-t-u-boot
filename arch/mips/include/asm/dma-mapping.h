/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2007
 * Stelian Pop <stelian@popies.net>
 * Lead Tech Design <www.leadtechdesign.com>
 */
#ifndef __ASM_MIPS_DMA_MAPPING_H
#define __ASM_MIPS_DMA_MAPPING_H

#include <linux/dma-direction.h>
#include <asm/io.h>

#define	dma_mapping_error(x, y)	0

static inline void *dma_alloc_coherent(size_t len, unsigned long *handle)
{
	void *p;

	p = memalign(ARCH_DMA_MINALIGN, len);
	*handle = virt_to_phys(p);

	return p;
}

static inline void dma_free_coherent(void *addr)
{
	free(addr);
}

#endif /* __ASM_MIPS_DMA_MAPPING_H */

