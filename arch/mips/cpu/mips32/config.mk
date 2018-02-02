#
# (C) Copyright 2003
# Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
#
# SPDX-License-Identifier:	GPL-2.0+
#

#
# Default optimization level for MIPS32
#
# Note: Toolchains with binutils prior to v2.16
# are no longer supported by U-Boot MIPS tree!
#
PLATFORM_CPPFLAGS += -DCONFIG_MIPS32 -march=mips32r2
PLATFORM_CPPFLAGS += -mabi=32 -DCONFIG_32BIT
ifdef CONFIG_SYS_BIG_ENDIAN
PLATFORM_LDFLAGS  += -m elf32btsmip
else
PLATFORM_LDFLAGS  += -m elf32ltsmip
endif

ifdef CONFIG_BAIKAL_FW
OBJCOPYFLAGS +=  -j .baikal_fw -j .baikal_spd
PLATFORM_CPPFLAGS += -DBAIKAL_FW_SECTION=${CONFIG_BAIKAL_FW} -DCONFIG_BAIKAL_SPD_OFF=0x7FC00 -DCONFIG_BAIKAL_FW_OFF=0x7D000
ifdef CONFIG_BAIKAL_SPD_ADDRESS
PLATFORM_CPPFLAGS += -DBAIKAL_READ_SPD
endif
endif

CONFIG_STANDALONE_LOAD_ADDR ?= 0x80200000 \
			       -T $(srctree)/examples/standalone/mips.lds
