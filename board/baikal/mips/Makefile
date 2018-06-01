#
# (C) Copyright 2013-2018
# Baikal Electronics OJSC
# Author: Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y += cpu.o
obj-y += clock.o
obj-y += lowlevel_init.o
obj-y += spi.o
obj-y += mem.o
obj-y += boot.o boot_misc.o ddr3_init.o pmu_misc.o ddr_spd.o

obj-$(CONFIG_BAIKAL_PLL) += clock_manager.o cmd_cpu.o
obj-$(CONFIG_CMD_SATA_PHY) += cmd_phy.o
obj-$(CONFIG_CMD_EXIT) += cmd_exit.o
obj-$(CONFIG_OF_LIBFDT) += fdt.o
#ifdef CONFIG_BAIKAL_LLENV
obj-$(CONFIG_BAIKAL_LLENV) += llenv32.o
KCFLAGS += -I$(KBUILD_SRC)/lib/baikal/llenv
#endif
obj-$(CONFIG_PCIE_DW) += pcie_dw.o
obj-$(CONFIG_BAIKAL_BFK) += bfk/
obj-$(CONFIG_BAIKAL_BFKX) += bfk/
obj-$(CONFIG_BAIKAL_BFK3) += bfk/
obj-$(CONFIG_BAIKAL_QEMU) += bfk/
obj-$(CONFIG_BAIKAL_FPGA) += bfk/
obj-$(CONFIG_TPLATFORMS_SFBT1) += sfbt1/
