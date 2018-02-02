#
# (C) Copyright 2013,2014
# Baikal Electronics OJSC
# Author: Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y = board.o
obj-y += cpu.o
obj-y += clock.o
obj-y += lowlevel_init.o
obj-y += spi.o
obj-y += boot.o boot_misc.o ddr3_init.o pmu_misc.o ddr_spd.o

obj-$(CONFIG_BAIKAL_PLL) += clock_manager.o cmd_cpu.o
obj-$(CONFIG_CMD_SATA_PHY) += cmd_phy.o
#ifdef CONFIG_BAIKAL_LLENV
obj-$(CONFIG_BAIKAL_LLENV) += llenv32.o
KCFLAGS += -I$(KBUILD_SRC)/lib/baikal/llenv
#endif
