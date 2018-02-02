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
#include <asm/arch/oem.h>
#include <asm/arch/sata.h>
#include <asm/arch/nvram.h>
#include <asm/arch/clock.h>
#include <asm/arch/clock_manager.h>
#include <board.h>
#include <config.h>
#include "llenv_spd.h"
#include "spi.h"


DECLARE_GLOBAL_DATA_PTR;

#define BRD15_BUS_NUM		0x01
#define BRD15_ADDR		0x51
#define BRD16_BUS_NUM		0x01
#define BRD16_ADDR		0x20

struct vendor_data vdata;


/* prototypes */
void designware_clear_phy_reset(ulong base);
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

/* Init board nvram */
void board_nvram_init(void)
{
	/* Check NVRAM */
	if (nvram_init())
		return;
	/* ARM generic boot options address is used as ptr to NVRAM data */
	gd->bd->bi_nvram = malloc(nvram_get_size() * sizeof(uchar));
	if (gd->bd->bi_nvram)
		nvram_read_block(gd->bd->bi_nvram, nvram_get_size());
}

#ifdef CONFIG_BOARD_EARLY_INIT_R
int board_early_init_r(void)
{
	/* init NVRAM */
	board_nvram_init();

#ifndef CONFIG_BAIKAL_BFK3
	int updated = 0;

	/* Get vendor data from EEPROM */
	if (oem_get_vendor_data(&vdata) < 0) {
		vdata.crc = 0;
		vdata.type = 0xff;
		vdata.revision = 0xff;
		vdata.serial = 0x0;
	}
#else
	vdata.crc = 0;
	vdata.type = 0xff;
	vdata.revision = 0x31;
	vdata.serial = 0x0;
#endif /* CONFIG_BAIKAL_BFK3 */

	if (vdata.type == 0xff) {
		vdata.type = oem_get_vendor_board(CONFIG_BAIKAL_NAME);
#ifndef CONFIG_BAIKAL_BFK3
		updated = 1;
#endif /* CONFIG_BAIKAL_BFK3 */
	}

	if (vdata.revision == 0xff) {
		vdata.revision = get_board_revision() & 0xff;
#ifndef CONFIG_BAIKAL_BFK3
		updated = 1;
#endif /* CONFIG_BAIKAL_BFK3 */
	}

#ifndef CONFIG_BAIKAL_BFK3
	if (updated)
		oem_set_vendor_data(&vdata);
#endif /* CONFIG_BAIKAL_BFK3 */

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

	/* Set mac addresses */
#ifdef CONFIG_BAIKAL_BFK3
	/* Set mac addresses based on CPU serial and rev */
	char flash_chip_id[BOOT_FLASH_ID_LEN * 2 + 1];
	flash_chip_id[BOOT_FLASH_ID_LEN * 2] = '\0';
	unsigned long brd_serial_id = 0;
	uint8_t data[BOOT_FLASH_ID_LEN];
	int err = llenv32_spi_bootid(data, sizeof(data));
	if (err) {
		printf("Error: Failed to read boot flash ID (%d)\n", err);
		brd_serial_id = getenv_ulong("cpu_serial", 10, 0);
	} else {
		for (int i = 0; i < BOOT_FLASH_ID_LEN; i++)
			sprintf(flash_chip_id + i * 2, "%02x", data[i]);

		printf("RomID: %s\n", flash_chip_id);
		/* Last 6 bytes of SPI boot flash chip ID */
		brd_serial_id = simple_strtol(flash_chip_id + (BOOT_FLASH_ID_LEN - 6), NULL, 16);
	}
	calc_and_set_macaddr(brd_serial_id, getenv_ulong("cpu_rev", 10, 0));
#else
	oem_setall_macaddr();
#endif /* CONFIG_BAIKAL_BFK3 */

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

#ifdef CONFIG_SATA_PHY_INIT
	unsigned int phy = getenv_ulong("sataphy", 10, 0);

	/* Init SATA PHY */
	if (phy) {
	sata_phy_init_val(0, phy);
	sata_phy_init_val(1, phy);
	}
	else
	sata_phy_init();
	printf("SATA:  PHY init complete\n");
#endif

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


/* Initialization of network */
int board_eth_init(bd_t *bis)
{
	int err = 0;

#ifdef CONFIG_BAIKAL_BFK3
#if defined(CONFIG_DESIGNWARE_ETH0_BASE)
	designware_clear_phy_reset(CONFIG_DESIGNWARE_ETH0_BASE);
#endif /* CONFIG_DESIGNWARE_ETH0_BASE */

#if defined(CONFIG_DESIGNWARE_ETH1_BASE)
	designware_clear_phy_reset(CONFIG_DESIGNWARE_ETH1_BASE);
#endif /* CONFIG_DESIGNWARE_ETH0_BASE */
#endif /* CONFIG_BAIKAL_BFK3 */

#if defined(CONFIG_DESIGNWARE_ETH0_BASE)
	if (designware_initialize(CONFIG_DESIGNWARE_ETH0_BASE,
			  PHY_INTERFACE_MODE_GMII) < 0)
		err |= (1 << 0);
#endif /* CONFIG_DESIGNWARE_ETH0_BASE */
#if defined(CONFIG_DESIGNWARE_ETH1_BASE)
	if (designware_initialize(CONFIG_DESIGNWARE_ETH1_BASE,
			  PHY_INTERFACE_MODE_GMII) < 0)
		err |= (1 << 1);
#endif /* CONFIG_DESIGNWARE_ETH1_BASE */
#if defined(CONFIG_DESIGNWARE_ETH2_BASE)
	if (designware_initialize(CONFIG_DESIGNWARE_ETH2_BASE,
			  PHY_INTERFACE_MODE_GMII) < 0)
		err |= (1 << 1);
#endif /* CONFIG_DESIGNWARE_ETH2_BASE */
	return (! err);
}



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

#if defined(CONFIG_LLENV32_SAVEENV)
int saveenv(void)
{
	llenv32_boot_ctrl(1);
	mdelay(100);

	int port = 0, line = 0;
	int ret = 0;

	void *cfg;
	env_t *env;

	/* spi config */
	cfg = llenv32_spi_store_cfg(port);
	if (!cfg) {
		ret = -1;
		return ret;
	}
	llenv32_spi_init(port);

	/* prepare */
	env = malloc(SPI_SECTOR_SIZE);
	if (!env) {
		ret = -2;
		goto _exit;
	}

	/* erase */
	if (llenv32_spi_erase(port, line, CONFIG_ENV_OFFSET)) {
		ret = -3;
		goto _exit;
	}

	/* export */
	if (env_export(env)) {
		ret = -4;
		goto _exit;
	}

	/* write */
	if (llenv32_spi_write_sector(port, line, CONFIG_ENV_OFFSET, env)) {
		ret = -5;
		goto _exit;
	}

_exit:
	if (llenv32_spi_restore_cfg (port, cfg)) {
		ret = -8;
	}

	if (env) {
		free(env);
	}

	llenv32_boot_ctrl(0);

	return ret;
}

#elif defined(CONFIG_BOOT_BASE)

#define BE_BC_CSR		0x00
#define BE_BC_MAR		0x04
#define BE_BC_DRID		0x08
#define BE_BC_VID		0x0C
#define BC_CSR_SPI_RDA	(1 << 8)
#define BC_CSR_BMODE	((1 << 0) | (1 <<1))

static struct spi_flash *env_flash;
int saveenv(void)
{
	int ret = 1;
	unsigned int reg;
	env_t env_new;

	/* Switch to SPI mode */
	reg = readl(CONFIG_BOOT_BASE + BE_BC_CSR);
	writel(reg | BC_CSR_SPI_RDA, CONFIG_BOOT_BASE + BE_BC_CSR);
	wmb();

	/* Probe flash if needed */
	if (!env_flash) {
		env_flash = spi_flash_probe(CONFIG_ENV_SPI_BUS,
			CONFIG_ENV_SPI_CS,
			CONFIG_ENV_SPI_MAX_HZ, CONFIG_ENV_SPI_MODE);
		if (!env_flash) {
			puts("Boot flash probe failed!\n");
			return 1;
		}
	}

	/* Get enviroment */
	ret = env_export(&env_new);
	if (ret)
		goto __done;

	/* Erase enviroment region */
	printf("spi_flash_erase: ofs=%x  size=%x\n", CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE);
	puts("Erasing SPI flash...");
	ret = spi_flash_erase(env_flash, CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE);
	if (ret)
		goto __done;
	puts("done\n");

	/* Write enviroment region */
	printf("spi_flash_write: ofs=%x  size=%x\n", CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE);
	puts("Writing to SPI flash...");
		ret = spi_flash_write(env_flash, CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE, &env_new);
	if (ret)
		goto __done;
	puts("done\n");

__done:
	/* Switch to transparent mode */
	writel(reg, CONFIG_BOOT_BASE + BE_BC_CSR);
	wmb();
	sync();
	/* Return result */
	return ret;
}

#else
int saveenv(void)
{
	puts("Flash is in read-only mode. Enviroment save is disabled.\n");
	return 1;
}
#endif

#ifdef CONFIG_CMD_EEPROM
int eeprom_write_enable (unsigned dev_addr, int state)
{
	return 0;
}
#endif
/* End of file board.c */


