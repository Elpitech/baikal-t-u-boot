/*
 * File:        board.c
 * Description: Board initialization for devices based on Baikal-T* SOCs
 * Authors:     Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 *              Ramil Zaripov <Ramil.Zaripov@baikalelectronics.com>
 *              Alexander Sazonov <Alexander.Sazonov@baikalelectronics.ru>
 * Copyright 2014-2017 Baikal Electronics JSC
 */


#include <common.h>
#include <environment.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/io.h>
#include <miiphy.h>
#include <i2c.h>
#include <netdev.h>
#include <malloc.h>
#include <asm/arch/sata.h>
#include <asm/arch/clock.h>
#include <asm/arch/clock_manager.h>
#include "spi.h"
#include <board.h>
#include <config.h>
#include "llenv_spd.h"

DECLARE_GLOBAL_DATA_PTR;

#define BRD15_BUS_NUM		0x01
#define BRD15_ADDR		0x51
#define BRD16_BUS_NUM		0x01
#define BRD16_ADDR		0x20

struct vendor_data vdata;

extern uint8_t  *const ddr_buffer0;
extern uint32_t *const ddr_buffer1;

/*
 * Get board revision
 */
int get_board_revision(void)
{
	int board_rev;

#ifndef CONFIG_BAIKAL_BFK3
	/* switch to correct I2C bus */
	int old_bus = I2C_GET_BUS();
	I2C_SET_BUS(BRD15_BUS_NUM);

	/* Read control value */
	if (i2c_probe(BRD15_ADDR) == 0)
		board_rev = 0x15;
	else
		board_rev = 0x10;

	I2C_SET_BUS(BRD16_BUS_NUM);
	if (i2c_probe(BRD16_ADDR) == 0)
		board_rev = 0x16;

	/* switch back to original I2C bus */
	I2C_SET_BUS(old_bus);
#else
	board_rev = 0x31;
#endif /* CONFIG_BAIKAL_BFK3 */

	return board_rev;
}

#ifdef CONFIG_BOARD_EARLY_INIT_R
int board_early_init_r(void)
{

	vdata.crc = 0;
	vdata.type = 0xff;
	vdata.revision = 0x31;
	vdata.serial = 0x0;

	if (vdata.revision == 0xff) {
		vdata.revision = get_board_revision() & 0xff;
		updated = 1;
	}

	printf("Rev:   %u.%u\n", (vdata.revision >> 4) & 0x0f, vdata.revision & 0x0f);

	if (vdata.serial != 0) {
		printf("S/N:   %llu\n", vdata.serial);
	}

	/* Return success */
	return 0;
}
#endif /* CONFIG_BOARD_EARLY_INIT_R */

#ifdef CONFIG_BOARD_LATE_INIT

#ifdef CONFIG_BAIKAL_BFK3
#define ETH_ID_1G_1		1
#define ETH_ID_1G_2		2
#define ETH_ID_10G		0
void calc_and_set_macaddr(int serial, int rev)
{
	u64 macaddr;
	char buffer[11];
	u8* b=(u8*)&macaddr;

	/* Ethernet 10G (eth0) */
	macaddr = CONFIG_BAIKAL_MAC_BASE | ((serial << 2) & 0xFFFFFC) | ETH_ID_10G;
	sprintf(buffer,"%02x:%02x:%02x:%02x:%02x:%02x",b[5],b[4],b[3],b[2],b[1],b[0]);
	setenv("ethaddr", buffer);

	/* Ethernet 1G (eth1) */
	macaddr = CONFIG_BAIKAL_MAC_BASE | ((serial << 2) & 0xFFFFFC) | ETH_ID_1G_1;
	sprintf(buffer,"%02x:%02x:%02x:%02x:%02x:%02x",b[5],b[4],b[3],b[2],b[1],b[0]);
	setenv("eth1addr", buffer);

	/* Ethernet 1G (eth2) */
	macaddr = CONFIG_BAIKAL_MAC_BASE | ((serial << 2) & 0xFFFFFC) | ETH_ID_1G_2;
	sprintf(buffer,"%02x:%02x:%02x:%02x:%02x:%02x",b[5],b[4],b[3],b[2],b[1],b[0]);
	setenv("eth2addr", buffer);
}
#endif /* CONFIG_BAIKAL_BFK3 */

#define BOOT_FLASH_ID_LEN 20

int board_late_init(void)
{
	char buf[32];

	/* Set board serial number */
	snprintf(buf, 20, "%llu", vdata.serial);
	setenv("board_serial", buf);

	/* Set board revision */
	snprintf(buf, 10, "%u.%u", (vdata.revision >> 4) & 0x0f,
			vdata.revision & 0x0f);
	setenv("board_rev", buf);

	/* Set board name */
	setenv("board_name", CONFIG_BAIKAL_NAME);

	/* Set default size of high memory size, value has been taken from SPD */
	sprintf(buf,"%x", get_ddr_highmem_size());
	setenv("memory_high_size", buf);

	return 0;
}
#endif /* CONFIG_BOARD_LATE_INIT */

/* Init SPI interface */
#if 0
void spi_init(void)
{
	/* Nothing todo here */
	return;
}
#endif


#ifdef CONFIG_BAIKAL_SPD_ADDRESS
void i2c_hw_init(int speed, int slaveaddr);

int baikal_read_spd(uint32_t addr, int alen, uint8_t *buffer, int len)
{
	int ret;

	/* Init I2C */
	i2c_hw_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);

	/* Init timer */
	(*(volatile unsigned long *)(CONFIG_SRAM_SPD_TIMESTAMP)) = 0;
	write_c0_cause(0);

	ret = i2c_read(CONFIG_BAIKAL_SPD_ADDRESS, addr, alen, buffer, len);
	*((int *)(CONFIG_DDR_INIT_RESULT_i2c)) = ret;

	return ret;
}
#endif



/* Add this function for transmit high memory size to common/image.c */

#define MAX_HIGHMEM_SIZE    0xDFFFFFF0
#define DDR3_SEG_SIZE		256


uint32_t get_ddr_highmem_size(void)
{
	uint32_t size;
	size = ddr_buffer1 [DDR3_SPD_SARSIZE1] + 1;
	size *= DDR3_SEG_SIZE * 1024 * 1024;

	if (size > MAX_HIGHMEM_SIZE)
		size = MAX_HIGHMEM_SIZE;

	return size;
}


uint32_t get_ddr_rank(void)
{
	if (ddr_buffer1 [DDR3_SPD_ADDRMAP0] == 0x1f)
		return 1;
	else
		return 2;
}

void print_ddr_regs (void)
{
	int i;
	uint32_t *p = ddr_buffer1;
	printf("DDRC/PUB reg base = 0x%x\n", (uint32_t)p);
	for (i = 0; i < 64; i++) {
		printf("reg(%02d) = 0x%08x\n", i, p[i]);
	}
}

void print_ddr_spd (void)
{
	int i;
	uint8_t *p = (uint8_t *)ddr_buffer0;
	printf("DDR SPD base = 0x%x\n", (uint32_t)p);
	for (i = 0; i < 256; i+= 2) {
		if ((i % 16) == 0)
			printf("\nspd(%02d):\t", i);
		printf("%02x%02x ", p[i], p[i+1]);
	}
	printf("\n");
}


phys_size_t initdram(int board_type)
{
	uint32_t rank = get_ddr_rank();
	uint32_t ddr_high_size = get_ddr_highmem_size();

#ifdef CONFIG_BAIKAL_PRINT_DDR
	print_ddr_regs();
	print_ddr_spd();
#endif /* CONFIG_BAIKAL_PRINT_DDR_REGS */

	printf("Rank = %d highmem = %d MiB lowmem = ", rank, (ddr_high_size / 1024 / 1024)); /* B -> MB */
	if (*((int *)(CONFIG_DDR_INIT_RESULT_v0)))
	{
		printf( /*lowmem*/ "...\n");
		printf("Baikal FW: DDR init failed v0=0x%x, v1=0x%x, step=0x%x, i2c=0x%x\n",
			*((int *)(CONFIG_DDR_INIT_RESULT_v0)),
			*((int *)(CONFIG_DDR_INIT_RESULT_v1)),
			*((int *)(CONFIG_DDR_INIT_RESULT_t7)),
			*((int *)(CONFIG_DDR_INIT_RESULT_i2c))
		);

#ifndef CONFIG_BAIKAL_FW_CONTINUE_ON_DDR_FAIL
		return 0;
#endif /* CONFIG_BAIKAL_FW_CONTINUE_ON_DDR_FAIL */
	}
	return CONFIG_SYS_SDRAM_SIZE;   /* Only one bank on lower 512 MB can be reached without MMU */
}

void board_add_ram_info(int use_default)
{
	bd_t *bd = gd->bd;

	/* Lowlevel DDR init should be here */
	bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 100;
	bd->bi_memstart = CONFIG_SYS_SDRAM_BASE;
	bd->bi_memsize = get_ram_size((void *)CONFIG_SYS_SDRAM_BASE,
					CONFIG_SYS_SDRAM_SIZE);

#ifdef CONFIG_SYS_SRAM_BASE
	bd->bi_sramstart = CONFIG_SYS_SRAM_BASE;
	bd->bi_sramsize = get_ram_size((void *)CONFIG_SYS_SRAM_BASE,
					CONFIG_SYS_SRAM_SIZE);
#endif


	bd->bi_flashstart = CONFIG_SYS_TEXT_BASE;
	bd->bi_flashsize = KSEG1 - KSEG0 - CPHYSADDR(CONFIG_SYS_TEXT_BASE);
	bd->bi_flashoffset = (CONFIG_SYS_MONITOR_BASE - CONFIG_SYS_TEXT_BASE);
}

/*
 * Show board name at startup
 */
int checkboard(void)
{
#if defined(CONFIG_BAIKAL_T1)
	puts("Board: Baikal-T1 " CONFIG_BAIKAL_NAME "\n");
#else
	puts("Board: Baikal-T " CONFIG_BAIKAL_NAME "\n");
#endif
	return 0;
}

#ifdef CONFIG_CMD_EEPROM
int eeprom_write_enable (unsigned dev_addr, int state)
{
	return 0;
}
#endif
/* End of file board.c */


