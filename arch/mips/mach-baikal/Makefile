obj-y	= lowlevel_init.o mem.o ddr3.o ddrc_init.o baikal_misc.o cpu.o
obj-$(CONFIG_PCI) += pci.o
ifeq ($(CONFIG_CUSTOM_SPD),y)
SPD_FILE := $(patsubst "%", SPD_%.o, $(CONFIG_CUSTOM_SPD_FILE))
obj-y += $(SPD_FILE)
$(obj)/SPD_%.o: $(srctree)/board/$(VENDOR)/spd/SPD_%.c
	$(call if_changed_rule,cc_o_c)

endif
