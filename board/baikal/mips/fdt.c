/*
 * File:        fdt.c
 * Description: FDT fixup methods
 * Created:     10 Nov 2016
 * Author:      Sergey Semin <sergey.semin@t-platforms.ru>
 * Copyright 2016 T-platforms
 *
 * U-boot works in a 'program' memory of MIPS architecture. In general it means,
 * there are five memory segments KUSEG, KSEG0 - KSEG3. These segments reflects
 * real physical memory space as: KUSEG - is accessible over MMU only by
 * unpriviliged mode, KSEG0 - mapped to first 512Mb of physical memory in a way
 * so can not be mapped by MMU, but can be cached, KSEG1 - the same as KSEG0
 * but can't be cached, KSEG2 and KSEG3 - each maps 512Mb accessible over MMU
 * in kernel mode only.
 * U-boot uses these segments transparently, so it thinks it works in physical
 * memory, which just starts from KSEG0 (0x80000000). One can notice, that
 * U-boot mostly operates  with memory within KSEG0/KSEG1 (0x80000000 -
 * 0xBFFFFFF). But Linux MIPS CSP is designed the way so KSEG0 and KSEG1 are
 * considered as virtually mapped contigious physical memory of kernel space.
 * In fact Linux approach is only correct, since it reflects the real physical
 * and virtual memory maps. Due to this inconsistency some addresses passed from
 * u-boot to Linux and vice-versa are inapproprietly considered. Particularly
 * it concerns fdt  related addresses like /memreserve/ and /linux,initrd-/
 * nodes, which must have physical memory according to standard.
 * Since It's too painful to make any design refactors to harmonize both
 * approachs, we need to use libfdt fixups to alter passed fdt.
 *
 * WARNING #1! There is method boot_fdt_add_mem_rsv_regions(), which is called
 * twice in the code:
 * 1) just before arch-specific bootm code is executed,
 * 2) in the arch specific code if image_setup_linux() method is called.
 * This method uses data from fdt /memreserve/ node to reserve memory in lmb.
 * Alas it is called before any libfdt initialization, which makes it impossible
 * to fixup
 *
 * WARNING #2! Alas U-boot doesn't analize /reserved-memory/ of passed fdt.
 * In addition it analizes /memreserve/ too late, so initrd and fdt are
 * relocated before memory declared in fdt is reserved. So get ready to see
 * memory overlap hidden errors, which may cause sudden Linux crashs =)
 */
#include <common.h>
#include <fdt_support.h>
#include <errno.h>
#include <libfdt.h>
#include <asm/io.h>

/*
 * Convert the passed property to be of physical address
 */
static int fdt_convert_prop(void *data, int len)
{
	int ret = 0;

	/* Copy the value from the data, convert and get it back */
	if (len == 8) {
		uint64_t val = fdt64_to_cpu(*(uint64_t *)data);
		val = cpu_to_fdt64(XPHYSADDR(val));
		memmove(data, &val, len);
	} else if (len == 4) {
		uint32_t val = fdt32_to_cpu(*(uint32_t *)data);
		val = cpu_to_fdt32(CPHYSADDR(val));
		memmove(data, &val, len);
	} else {
		printf("Unsupported linut,initrd-* property len: %d", len);
		ret = -EINVAL;
	}

	return ret;
}

/*
 * Translate the /memreserve/ nodes to be within KSEG0
 */
int arch_fixup_fdt(void *fdt_blob)
{
	uint64_t addr, size;
	int n, total, ret;

	/*
	 * Walk through all the memreserve nodes deleting a first one and
	 * adding a translated one to tail
	 */
	total = fdt_num_mem_rsv(fdt_blob);
	for (n = 0; n < total; n++) {
		/* FDT header check may fail */
		ret = fdt_get_mem_rsv(fdt_blob, 0, &addr, &size);
		if (ret != 0) {
			puts("Failed to get memreserve from fdt");
			return ret;
		}
		/* Do nothing if the address is already virtual */
		if (KSEGX(addr) != 0)
			continue;
		/* Delete the very first memreserve node */
		ret = fdt_del_mem_rsv(fdt_blob, 0);
		if (ret != 0) {
			puts("Failed to clear first memreserve node");
			return ret;
		}
		/* Add translated /memreserve/ node at the tail */
		addr = (uintptr_t)phys_to_virt(addr);
		ret = fdt_add_mem_rsv(fdt_blob, addr, size);
		if (ret != 0) {
			puts("Failed to get memreserve back");
			return ret;
		}
	}

	return 0;
}

/*
 * Translate the /memreserve/ nodes back to have physical addresses
 */
int ft_verify_fdt(void *fdt_blob)
{
	uint64_t addr, size;
	int n, total, ret, len;
	int chosen_node;
	void *data;

	/*
	 * Walk through all the memreserve nodes deleting a first one and
	 * adding a translated one to tail
	 */
	total = fdt_num_mem_rsv(fdt_blob);
	for (n = 0; n < total; n++) {
		/* FDT header check may fail */
		ret = fdt_get_mem_rsv(fdt_blob, 0, &addr, &size);
		if (ret != 0) {
			puts("Failed to get memreserve from fdt");
			return false;
		}
		/* Do nothing if the address is already physical */
		if (KSEGX(addr) == 0)
			continue;
		/* Delete the very first memreserve node */
		ret = fdt_del_mem_rsv(fdt_blob, 0);
		if (ret != 0) {
			puts("Failed to clear first memreserve node");
			return false;
		}
		/* Add translated /memreserve/ node at the tail */
		addr = virt_to_phys((void *)(uintptr_t)addr);
		ret = fdt_add_mem_rsv(fdt_blob, addr, size);
		if (ret != 0) {
			puts("Failed to get memreserve back");
			return false;
		}
	}

	/*
	 * Replace virtual address set to linux,initrd-start and
	 * linux,initrd-end with proper physical addresses
	 */
        chosen_node = fdt_subnode_offset(fdt_blob, 0, "chosen");
        if (chosen_node < 0) {
		puts("Failed to get chosen node to alter initrd address");
		return false;
	}

	/* Retrieve the linux,initrd-start property */
	data = fdt_getprop_w(fdt_blob, chosen_node, "linux,initrd-start", &len);
	if (!data) {
		puts("Failed to get linux,initrd-start property");
		return false;
	}
	/* Convert the retrieved property */
	ret = fdt_convert_prop(data, len);
	if (ret < 0)
		return false;

	/* Retrieve the linux,initrd-end property */
	data = fdt_getprop_w(fdt_blob, chosen_node, "linux,initrd-end", &len);
	if (!data) {
		puts("Failed to get linux,initrd-end property");
		return false;
	}
	/* Convert the retrieved property */
	ret = fdt_convert_prop(data, len);
	if (ret < 0)
		return false;

	return true;
}

/* End of file fdt.c */
