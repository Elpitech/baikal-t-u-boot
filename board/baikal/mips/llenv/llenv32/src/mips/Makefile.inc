LLENV_DIR=..

include ${LLENV_DIR}/Makefile.common

CFLAGS += -DARCH_MIPS -mel -Werror -G0 -fno-common
SFLAGS += -DARCH_MIPS -mel -Werror -G0 -fno-common -mdsp

SOURCE_DDR_FILES_S = mips/ddr_1066.S mips/ddr_1600.S mips/ddr_haps.S

ifeq (${USE_DDR_SPD},yes)
SOURCE_DDR_FILES_S += mips/ddr_spd.S
endif

TS_LIST = ${TS_DEV_LIST} ${TS_MIPS_LIST}


SOURCE_ARCH_FILES_C := mips/mmu_misc.c
SOURCE_ARCH_FILES_S :=  mips/msafpe_handler.S mips/trap_irq.S 
ifneq (${BUILD_FOR_UBOOT_LIB},yes)
SOURCE_ARCH_FILES_C += mips/gic_misc.c
SOURCE_ARCH_FILES_S += mips/boot_misc.S mips/spinlock.S
endif

ifeq (${INIT_DDR}, yes)
SOURCE_ARCH_FILES_S +=  $(SOURCE_DDR_FILES_S)
endif
ifeq (${EARLY_PRINTK}, yes)
SOURCE_ARCH_FILES_S += mips/early_printk.S
endif

SOURCE_ARCH_FILES_C += $(addprefix mips/tests/, $(addsuffix .c, $(TS_MIPS_LIST)))

mips/ddr_1066.S mips/ddr_1600.S mips/ddr_haps.S mips/ddr_spd.S: mips/ddr_%.S: mips/ddr.template
	rm -fr $@
	echo "/* This file is generated from template, do not edit. */" > $@
	sed -e 's/__HNDL_NAME__/$*/g; s/__PREFIX__/\U$*/' mips/ddr.template >> $@

