#
# Low-level and POST tests.
#
# Copyright (C) 2014 Baikal Electronics.
#
# Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
#
# Description:
#

LLENV32_DIR=..
include ${LLENV32_DIR}/Makefile.common

BROM_FLAGS += \
			-DBROM \
			-DDONT_DECLARE_TC \
			-Werror \
			-Os \
			-ffunction-sections \
			-fdata-sections \
			-fno-zero-initialized-in-bss


PATH_COMMON_FILES = ./common
PATH_DEV_FILES = ./dev

TEST_ARCH_DIR = ${TEST_ARCH}

SOURCE_COMMON_FILES_C := common/common.c common/strings.c
ifeq (${RUN_SHELL}, yes)
SOURCE_COMMON_FILES_C += common/cmdline.c
endif
ifeq (${DEBUG}, yes)
SOURCE_COMMON_FILES_C += common/output.c
endif

SOURCE_DEV_FILES := dev/pmu_misc.c

ifneq (${BUILD_FOR_UBOOT_LIB},yes)
SOURCE_DEV_FILES += dev/spi_misc.c dev/timer_handler.c dev/pvt_misc.c
ifeq (${DEBUG}, yes)
SOURCE_DEV_FILES += dev/uart_misc.c
endif
endif

ifeq ($(findstring uart, ${TS_DEV_LIST}),uart)
SOURCE_DEV_FILES += dev/uart_misc.c
endif

ifeq ($(findstring xgmac, ${TS_DEV_LIST}), xgmac)
SOURCE_DEV_FILES += dev/xgmac_misc.c dev/marvell.c
endif

ifeq ($(findstring pcie, ${TS_DEV_LIST}), pcie)
SOURCE_DEV_FILES += dev/pcie_misc.c
endif

#SOURCE_DEV_FILES += dev/tests/payload/%.c
SOURCE_DEV_FILES += $(addprefix dev/tests/, $(addsuffix .c, $(TS_DEV_LIST)))

OBJS_FILES = $(SOURCE_ARCH_FILES_C:%.c=%.o) $(SOURCE_ARCH_FILES_S:%.S=%.o) \
                $(SOURCE_DEV_FILES:%.c=%.o) $(SOURCE_COMMON_FILES_C:%.c=%.o)

ifeq (${USE_DDR_SPD},yes)
OBJS_FILES += ddr3_parse_spd.o ddr3_spd.o
endif

#payload for spi test
OBJS_FILES += $(patsubst %.c,%.o,$(wildcard ${PATH_PAYLOAD}/*.c))
OBJS_FILES += $(patsubst %.S,%.o,$(wildcard ${PATH_PAYLOAD}/*.S))
OBJS_FILES += $(patsubst %.s,%.o,$(wildcard ${PATH_PAYLOAD}/*.s))

# Add settings from the command line.
CFLAGS := $(CFLAGS_CONFIG) -fno-store-merging
SFLAGS := $(SFLAGS_CONFIG) -fno-store-merging

CFLAGS += $(CFLAGS_EXTRA)


all: clean libllenv.a

brom: CFLAGS += $(BROM_FLAGS)
brom: SFLAGS += $(BROM_FLAGS)
brom: all

uboot_lib: OBJS_FILES += tmp/conf.o
uboot_lib: clean tmp/conf.o
uboot_lib: libllenv.a

tmp/conf.o:
	@echo -------------------------
	@echo -   $@
	@echo -------------------------
	rm -fr ./tmp
	mkdir tmp
	../standalone/conf.sh $(TS_LIST)
	$(CC) $(CFLAGS) -c tmp/conf.c -o tmp/conf.o

include ${TEST_ARCH_DIR}/Makefile.inc

ddr3_parse_spd.o: spd_prepare $(PATH_DDR_SPD)/ddr3_init.c
	@echo -------------------------
	@echo -   $@
	@echo -------------------------
	$(CC) $(CFLAGS) -c $(PATH_DDR_SPD)/ddr3_init.c -o $@

ddr3_spd.o: $(PATH_DDR_SPD)/ddr3_spd.c
	@echo -------------------------
	@echo -   ddr3_spd.o
	@echo -------------------------
	$(CC) $(CFLAGS) -c $(PATH_DDR_SPD)/ddr3_spd.c -o $@

spd_prepare:
	@echo -------------------------
	@echo -   $@
	@echo -------------------------
	# make -C $(PATH_DDR_SPD) DDR_GEOMETRY=${DDR_GEOMETRY} GEN_DDR_SPD=${GEN_DDR_SPD}
	make -C $(PATH_DDR_SPD) DDR_GEOMETRY=${DDR_GEOMETRY} FACTORY_SPD=${FACTORY_SPD} DDR_SPEED_BIN=${DDR_SPEED_BIN}


./dev/tests/dma_cfgreg.c ./dev/tests/dma_crypto.c: ./dev/tests/dma_%.c : ./dev/tests/dma.template
	@echo '========================'
	@echo -   $@
	@echo '========================'
	rm -fr $@
	echo "/* This file is generated from template, do not edit. */\n" > $@
	sed -e 's/__TSUITE_NAME__/dma_$*/g; s/__PREFIX__/\U$*/g' ./dev/tests/dma.template >> $@



# ${PATH_PAYLOAD}/%.o: ${PATH_PAYLOAD}/%.c
	# @echo -------------------------
	# @echo -   $@
	# @echo -------------------------
	# PATH_DDR_SPD = ${LLENV32_DIR}/../ddr
	# $(CC) $(CFLAGS) -c ${PATH_PAYLOAD}/%.c -o $@
	# $(CC) $(CFLAGS) -c $(PATH_DDR_SPD)/ddr3_spd.c -o $@


%.o: %.c
	@echo -------------------------
	@echo -   $@
	@echo -------------------------
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	@echo -------------------------
	@echo -   $@
	@echo -------------------------
	$(CC) $(SFLAGS) -c $< -o $@

libllenv.a: $(OBJS_FILES) $(LINK_DEPS)
	@echo -------------------------
	@echo -   $@
	@echo -------------------------
	@$(AR) srv $@ $(OBJS_FILES)
	@${OBJDUMP} -Dz $@ > $@.dump

clean:
	@echo -------------------------
	@echo -   $@
	@echo -------------------------
	rm -fr  									\
		libllenv.a 								\
		libllenv.a.dump
	rm -fr	./tmp
	rm -fr 										\
		$(PATH_DEV_FILES)/tests/dma_cfgreg.c 	\
		$(PATH_DEV_FILES)/tests/dma_crypto.c

	rm -fr 					  					\
		*.o  									\
		$(PATH_DEV_FILES)/*.o  					\
		$(PATH_DEV_FILES)/tests/*.o  			\
		$(TEST_ARCH_DIR)/*.o  					\
		$(TEST_ARCH_DIR)/tests/*.o 				\
		${PATH_PAYLOAD}/*.o 					\
		common/*.o

	# rm -fr *.o libllenv.a libllenv.a.dump
	# rm -fr $(PATH_DEV_FILES)/*.o $ (PATH_DEV_FILES)/tests/*.o $ (TEST_ARCH_DIR)/*.o $ (TEST_ARCH_DIR)/tests/*.o $ (CLEAN) $ (PATH_DEV_FILES)/tests/dma_cfgreg.c $ (PATH_DEV_FILES)/tests/dma_crypto.c
