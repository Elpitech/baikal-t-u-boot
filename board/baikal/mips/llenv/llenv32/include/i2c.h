
#ifndef __I2C_H__
#define __I2C_H__

#define I2C_PORTS_NUM		2
#define I2C_PORT0		0
#define I2C_PORT1		1


#define I2C_PORT_START		I2C_PORT0
#define I2C_PORT_END		I2C_PORT1

#define I2C_PORT(p)		(I2C_BASE + (p)*IPORT_OFF)

/* I2C registers. */
#define IC_CON(p)			(I2C_PORT(p) + 0x0) /* I2C Control  */
#define IC_TAR(p)			(I2C_PORT(p) + 0x4) /* I2C Target Address  */
#define IC_SAR(p)			(I2C_PORT(p) + 0x8) /* I2C Slave Address  */
#define IC_HS_MADDR(p)			(I2C_PORT(p) + 0xc) /* I2C High Speed Master Mode Code Address */
#define IC_DATA_CMD(p)			(I2C_PORT(p) + 0x10) /* I2C Rx/Tx Data Buffer and Command Register */
#define IC_SS_SCL_HCNT(p)		(I2C_PORT(p) + 0x14) /* Standard Speed I2C Clock SCL High Count */
#define IC_SS_SCL_LCNT(p)		(I2C_PORT(p) + 0x18) /* Standard Speed I2C Clock SCL Low Count */
#define IC_FS_SCL_HCNT(p)		(I2C_PORT(p) + 0x1c) /* Fast Speed I2C Clock SCL High Count */
#define IC_FS_SCL_LCNT(p)		(I2C_PORT(p) + 0x20) /* Fast Speed I2C Clock SCL Low Count */
#define IC_HS_SCL_HCNT(p)		(I2C_PORT(p) + 0x24) /* High Speed I2C Clock SCL High Count */
#define IC_HS_SCL_LCNT(p)		(I2C_PORT(p) + 0x28) /* High Speed I2C Clock SCL Low Count */
#define IC_INTR_STAT(p)			(I2C_PORT(p) + 0x2c) /* I2C Interrupt Status  */
#define IC_INTR_MASK(p)			(I2C_PORT(p) + 0x30) /* I2C Interrupt Mask  */
#define IC_RAW_INTR_STAT(p)		(I2C_PORT(p) + 0x34) /* I2C Raw Interrupt Status  */
#define IC_RX_TL(p)			(I2C_PORT(p) + 0x38) /* I2C Receive FIFO Threshold  */
#define IC_TX_TL(p)			(I2C_PORT(p) + 0x3c) /* I2C Transmit FIFO Threshold  */
#define IC_CLR_INTR(p)			(I2C_PORT(p) + 0x40) /* Clear Combined and Individual Interrupt */
#define IC_CLR_RX_UNDER(p)		(I2C_PORT(p) + 0x44) /* Clear RX_UNDER Interrupt  */
#define IC_CLR_RX_OVER(p)		(I2C_PORT(p) + 0x48) /* Clear RX_OVER Interrupt  */
#define IC_CLR_TX_OVER(p)		(I2C_PORT(p) + 0x4c) /* Clear TX_OVER Interrupt  */
#define IC_CLR_RD_REQ(p)		(I2C_PORT(p) + 0x50) /* Clear RD_REQ Interrupt  */
#define IC_CLR_TX_ABRT(p)		(I2C_PORT(p) + 0x54) /* Clear TX_ABRT Interrupt  */
#define IC_CLR_RX_DONE(p)		(I2C_PORT(p) + 0x58) /* Clear RX_DONE Interrupt  */
#define IC_CLR_ACTIVITY(p)		(I2C_PORT(p) + 0x5c) /* Clear ACTIVITY Interrupt  */
#define IC_CLR_STOP_DET(p)		(I2C_PORT(p) + 0x60) /* Clear STOP_DET Interrupt  */
#define IC_CLR_START_DET(p)		(I2C_PORT(p) + 0x64) /* Clear START_DET Interrupt  */
#define IC_CLR_GEN_CALL(p)		(I2C_PORT(p) + 0x68) /* Clear GEN_CALL Interrupt  */
#define IC_ENABLE(p)			(I2C_PORT(p) + 0x6c) /* I2C Enable  */
#define IC_STATUS(p)			(I2C_PORT(p) + 0x70) /* I2C Status  */
#define IC_TXFLR(p)			(I2C_PORT(p) + 0x74) /* I2C Transmit FIFO Level  */
#define IC_RXFLR(p)			(I2C_PORT(p) + 0x78) /* I2C Receive FIFO Level  */
#define IC_SDA_HOLD(p)			(I2C_PORT(p) + 0x7c) /* I2C SDA Hold  */
#define IC_TX_ABRT_SOURCE(p)		(I2C_PORT(p) + 0x80) /* I2C Transmit Abort Source  */
#define IC_DMA_CR(p)			(I2C_PORT(p) + 0x88) /* DMA Control  */
#define IC_DMA_TDLR(p)			(I2C_PORT(p) + 0x8c) /* DMA Transmit Data Level  */
#define IC_DMA_RDLR(p)			(I2C_PORT(p) + 0x90) /* I2C Receive Data Level  */
#define IC_SDA_SETUP(p)			(I2C_PORT(p) + 0x94) /* I2C SDA Setup  */
#define IC_ACK_GENERAL_CALL(p)		(I2C_PORT(p) + 0x98) /* I2C ACK General Call  */
#define IC_ENABLE_STATUS(p)		(I2C_PORT(p) + 0x9c) /* I2C Enable Status  */
#define IC_FS_SPKLEN(p)			(I2C_PORT(p) + 0xa0) /* I2C SS and FS spike suppression limit  */
#define IC_HS_SPKLEN(p)			(I2C_PORT(p) + 0xa4) /* I2C HS spike suppression limit  */
#define IC_COMP_PARAM_1(p)		(I2C_PORT(p) + 0xf4) /* Component Parameter  */
#define IC_COMP_VERSION(p)		(I2C_PORT(p) + 0xf8) /* I2C Component Version  */
#define IC_COMP_TYPE(p)			(I2C_PORT(p) + 0xfc) /* I2C Component Type  */
 
/* IC_ENABLE */
#define IC_ENABLE_EN		(1 << 0)
#define IC_ENABLE_AB		(1 << 1)

/* IC_CON */
#define IC_CON_MASTER_MODE	(1 << 0)
#define IC_CON_SPEED_OFFSET	(1)
#define IC_CON_SPEED_MASK	(3 << IC_CON_SPEED_OFFSET)
#define IC_CON_SPEED_STD	1	/* standard mode, 0 to 100 Kb/s */
#define IC_CON_SPEED_FAST	2	/* fast mode< <= 1Mb/s  */
#define IC_CON_SPEED_HIGH	3	/* high speed mode <= 3.4Mb/s */
#define IC_CON_10BITADDR_SLAVE	(1 << 3)
#define IC_CON_10BITADDR_MASTER	(1 << 4)
#define IC_CON_RESTART_EN	(1 << 5)
#define IC_CON_SLAVE_DISABLE	(1 << 6)

#define ETH_PLL_FREQ		125 /* Default 1GbE Frequency (MHz) */
#define MEGA_TO_KILO		1000

/* I2C frequencies in KHz */
#define IC_STANDARD_SPD		100
#define IC_FAST_SPD		400
#define IC_HIGH_SPD		1000

/* Register reset values. */
#define IC_SS_SCL_HCNT_VAL	0x1f4
#define IC_SS_SCL_LCNT_VAL	0x24c
#define IC_FS_SCL_HCNT_VAL	0x4b
#define IC_FS_SCL_LCNT_VAL	0xa3
#define IC_HS_SCL_HCNT_VAL	0x8
#define IC_HS_SCL_LCNT_VAL	0x14
#define IC_INTR_MASK_VAL	0x8ff
#define IC_FS_SPKLEN_VAL	0x7
#define IC_HS_SPKLEN_VAL	0x2
#define IC_COMP_PARAM_1_VAL	0x707ee
#define IC_COMP_VERSION_VAL	0x3132312a
#define IC_COMP_TYPE_VAL	0x44570140

#define              IC_CON_VAL		0x0000007f
#define              IC_TAR_VAL		0x00001055
#define              IC_SAR_VAL		0x00000055
#define         IC_HS_MADDR_VAL		0x00000001
#define         IC_DATA_CMD_VAL		0x00000000
#define        IC_INTR_STAT_VAL		0x00000000
#define    IC_RAW_INTR_STAT_VAL		0x00000000
#define            IC_RX_TL_VAL		0x00000000
#define            IC_TX_TL_VAL		0x00000000
#define         IC_CLR_INTR_VAL		0x00000000
#define     IC_CLR_RX_UNDER_VAL		0x00000000
#define      IC_CLR_RX_OVER_VAL		0x00000000
#define      IC_CLR_TX_OVER_VAL		0x00000000
#define       IC_CLR_RD_REQ_VAL		0x00000000
#define      IC_CLR_TX_ABRT_VAL		0x00000000
#define      IC_CLR_RX_DONE_VAL		0x00000000
#define     IC_CLR_ACTIVITY_VAL		0x00000000
#define     IC_CLR_STOP_DET_VAL		0x00000000
#define    IC_CLR_START_DET_VAL		0x00000000
#define     IC_CLR_GEN_CALL_VAL		0x00000000
#define           IC_ENABLE_VAL		0x00000000
#define           IC_STATUS_VAL		0x00000006
#define            IC_TXFLR_VAL		0x00000000
#define            IC_RXFLR_VAL		0x00000000
#define         IC_SDA_HOLD_VAL		0x00000027
#define   IC_TX_ABRT_SOURCE_VAL		0x00000000
#define           IC_DMA_CR_VAL		0x00000000
#define         IC_DMA_TDLR_VAL		0x00000000
#define         IC_DMA_RDLR_VAL		0x00000000
#define        IC_SDA_SETUP_VAL		0x00000021
#define IC_ACK_GENERAL_CALL_VAL		0x00000001
#define    IC_ENABLE_STATUS_VAL		0x00000000


#define READ_I2C_REG(r)       READ_MEMORY_REG(r)
#define WRITE_I2C_REG(r, v)   WRITE_MEMORY_REG(r, v)

/* Define testing address of I2C slave.  */
#define TEST_SLAVE_ADDR		0x7
#define TEST_IRQ_MASK		0xFFF /* From 0-th bit to 11-th bit. */

/* Address of I2C EEPROM */
#define CHIP_1	0x50
#define CHIP_2	0x51
#define CHIP_3  0xA0
#define CHIP_4  0xA2

#ifndef __ASSEMBLER__
const static int eeprom_chips[] = {
			CHIP_1, CHIP_2, CHIP_3, CHIP_4
			};
#endif

/* IC CMD definitions */
#define IC_CMD_RW		0x100
#define IC_CMD_STOP		0x200
#define IC_CMD_RESTART		0x400

/* IC DMA configure */
#define IC_DMA_CR_TDMAE		(1 << 1)
#define IC_DMA_CR_RDMAE		(1 << 0)

/* i2c interrupt status register definitions */
#define IC_GEN_CALL		0x0800
#define IC_START_DET		0x0400
#define IC_STOP_DET		0x0200
#define IC_ACTIVITY		0x0100
#define IC_RX_DONE		0x0080
#define IC_TX_ABRT		0x0040
#define IC_RD_REQ		0x0020
#define IC_TX_EMPTY		0x0010
#define IC_TX_OVER		0x0008
#define IC_RX_FULL		0x0004
#define IC_RX_OVER 		0x0002
#define IC_RX_UNDER		0x0001

/* i2c status register  definitions */
#define IC_STATUS_SA		0x0040
#define IC_STATUS_MA		0x0020
#define IC_STATUS_RFF		0x0010
#define IC_STATUS_RFNE		0x0008
#define IC_STATUS_TFE		0x0004
#define IC_STATUS_TFNF		0x0002
#define IC_STATUS_ACT		0x0001
/* 
 * TEST_I2C_REG_ID macro returns 0 if the reg register is to equal to
 * its reg_val default value.
 */
#define TEST_I2C_REG_ID TEST_MEM_REG_ID
#define TEST_I2C_REG_VAL TEST_MEM_REG_ID


/* 
 * TEST_I2C_READ macro returns 0 if the reg register can be read.
 */
#define TEST_I2C_READ   TEST_READ_MEM_REG

#ifndef __ASSEMBLER__
void llenv32_i2c_handler (unsigned int iar, unsigned int aiar);
int llenv32_i2c_read(int port, const unsigned char chip,  unsigned int offset, int len, char *buf);
int llenv32_i2c_write(int port, const unsigned char chip,  unsigned int offset, int len, char *buf);
void i2c_set_speed(const int i2c_spd, int port);
#endif

#endif /* __I2C_H__ */
