/*
 * (C) Copyright 2003
 * Wolfgang Denk Engineering, <wd@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#if defined(CONFIG_64BIT)
#define PTR_COUNT_SHIFT	3
#else
#define PTR_COUNT_SHIFT	2
#endif

OUTPUT_ARCH(mips)
ENTRY(_start)
SECTIONS
{
	. = 0x00000000;

	. = ALIGN(4);
	.text : {
		*(.text*)
#if defined(BAIKAL_READ_SPD)
		KEEP (*(.baikal_read_spd))
#endif
	}

	. = ALIGN(4);
	.rodata : {
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
	}

	. = ALIGN(4);
	.data : {
		*(.data*)
	}

	. = .;
	_gp = ALIGN(16) + 0x7ff0;

	.got : {
		*(.got)
	}

	num_got_entries = SIZEOF(.got) >> PTR_COUNT_SHIFT;

	. = ALIGN(4);
	.sdata : {
		*(.sdata*)
	}

	. = ALIGN(4);
	.u_boot_list : {
		KEEP(*(SORT(.u_boot_list*)));
	}

	. = ALIGN(4);
	__image_copy_end = .;
	__init_end = .;

	.rel.dyn : {
		__rel_dyn_start = .;
		*(.rel.dyn)
		__rel_dyn_end = .;
	}

	_end = .;

	.bss __rel_dyn_start (OVERLAY) : {
		__bss_start = .;
		*(.sbss.*)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = .;
	}

	.dynsym _end : {
		*(.dynsym)
	}

	.dynbss : {
		*(.dynbss)
	}

	.dynstr : {
		*(.dynstr)
	}

	.plt : {
		*(.plt)
	}

	.hash : {
		*(.hash)
	}

	.MIPS.stubs : {
		*(.MIPS.stubs)
	}

#if defined(BAIKAL_FW_SECTION)
	. = _start + CONFIG_BAIKAL_FW_OFF;
	.baikal_fw : {
		__start_baikal_fw = .;
		KEEP (BAIKAL_FW_SECTION (.baikal_fw*))
	}

	. = _start + CONFIG_BAIKAL_SPD_OFF;
	.baikal_spd : {
#if defined(BAIKAL_READ_SPD)
		LONG(ABSOLUTE(baikal_read_spd));
#endif
		__start_baikal_spd = .;
		KEEP (BAIKAL_FW_SECTION (.baikal_spd*))
	}
#endif


	/* ----------------- */
	/* free_space: 0x300 */
	/* ----------------- */
	.gnu : {		/* 0x1e0 */
		*(.gnu*)
	}

	.dynamic : {	/* 0xc0 */
		*(.dynamic)
	}

	.interp : {
		*(.interp)  /* 0x13 */
	}

}
