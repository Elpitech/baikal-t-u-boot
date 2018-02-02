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
obj-y += ddr3_init.o ddr_spd.o

obj-$(CONFIG_BAIKAL_PLL) += clock_manager.o cmd_cpu.o
