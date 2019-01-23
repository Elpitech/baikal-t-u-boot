#
# Low-level and POST tests.
#
# Copyright (C) 2015 Baikal Electronics.
#
# Author: Ekaterina Skachko <Ekaterina.Skachko@baikalelectronics.com>
#

PATH_SRC = ./src
PATH_STANDALONE = ./standalone
PATH_UBOOT_FW = ./uboot_fw
BROM_TEXT_ADDR = 0x9FC01400

all: clean build

config_mips:
	cp conf/config.mips .config

config_arm:
	cp conf/config.arm .config

build:
	make -C $(PATH_SRC)
	make -C $(PATH_STANDALONE)

uboot_lib:
	make -C $(PATH_SRC) uboot_lib

ubootfw:
	make -C $(PATH_SRC)
	make -C $(PATH_UBOOT_FW)

brom:
	make brom -C $(PATH_SRC)

tcs:
	make -C $(PATH_SRC) CFLAGS_EXTRA=-DDONT_DECLARE_TC
	make tcs -C $(PATH_STANDALONE)

clean:
	make clean -C $(PATH_SRC)
	make clean -C $(PATH_STANDALONE)
	make clean -C $(PATH_UBOOT_FW)
