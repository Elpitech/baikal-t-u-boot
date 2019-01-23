
#ifndef __IRQ_MAP_MIPS_H__
#define __IRQ_MAP_MIPS_H__

/* GIC */
#define WEDGE_IRQ		(1 << 3) /* HW3 */
#define WEDGE_GIC_IRQ		3
#define WEDGE_CORE		CPU_ACTIVE /* Send interrupt to the active core. */

/* GIC WD */
#define WD_IRQ			(1 << 4) /* HW4 */

/* APB Terminator IRQ */
#define APB_TERM_IRQ		16
#define APB_IRQ			(1 << 5)

/* WDT IRQ */
#define WDT_IRQ			(1 << 5)
#define WD_GIC_IRQ		17

/* GPIO IRQ */
#define GPIO_IRQ		(1 << 5)
#define GPIO_GIC_IRQ		19

/* Software IRQs. */
#define SW_IRQ0			(1 << 0) /* IP0 */
#define SW_IRQ1			(1 << 1) /* IP0 */
#define EIC_SW_BASE		20
#define EIC_SW_IRQ0		(EIC_SW_BASE + 1)
#define EIC_SW_IRQ1		(EIC_SW_BASE + 2)

/* RTC IRQ. */
#define RTC_IRQ			(1 << 5) /* TODO */

/* Internal timer IRQ */
#define TIMER_COUNT_IRQ		(1 << 5) /* HW5 */
#define TIMER_COUNT_GIC_IRQ	5

/* dw timer IRQs */
#define TIMER_IRQ1		(1 << 0) /* HW0 */
#define TIMER_IRQ2		(1 << 0) /* TODO */
#define TIMER_IRQ3		(1 << 0) /* TODO */
#define TIMER_GIC_IRQ1		24
#define TIMER_GIC_IRQ2		25
#define TIMER_GIC_IRQ3		26

/* I2C IRQs */
#define PMU_I2C_IRQ		(1 << 2)
#define I2C_IRQ_0		(1 << 3) /* HW3 */
#define I2C_IRQ_1		(1 << 4) /* TODO */
#define PMU_I2C_GIC_IRQ		32
#define I2C_GIC_IRQ_0		33
#define I2C_GIC_IRQ_1		34

/* SPI IRQ */
#define SPI_IRQ_0		(1 << 2) /* HW2 */
#define SPI_IRQ_1               (1 << 3)
#define SPI_GIC_IRQ_0             40
#define SPI_GIC_IRQ_1             41

/* UART IRQ */
#define UART_IRQ_1		(1 << 1) /* HW1 */
#define UART_IRQ_2		(1 << 2) /* HW1 */
#define UART_GIC_IRQ_1		48
#define UART_GIC_IRQ_2		49

/* DMA_IRQ */
#define CFGREG_DMAC_GIC_IRQ	56
#define CFGREG_DMAC_IRQ		(1 << 1)

/* SATA IRQ */
#define SATA_IRQ		(1 << 4) /* HW4 */
#define SATA_GIC_IRQ		64

/* USB IRQ */
#define USB_IRQ                (1 << 3) /* HW3 */
#define USB_GIC_IRQ            68

/* GMAC IRQ */
#define GMAC_IRQ_0		(1 << 4) /* HW4 */
#define GMAC_IRQ_1              (1 << 5) /* HW5 */
#define GMAC_GIC_IRQ_0		72
#define GMAC_GIC_IRQ_1		73

/* XGMAC IRQ */
#define XGMAC_IRQ               54 // (1 << 0)
#define XGMAC_TX0_IRQ           55 // (1 << 1)
#define XGMAC_TX1_IRQ           56 // (1 << 2)
#define XGMAC_RX0_IRQ           57 // (1 << 3)
#define XGMAC_RX1_IRQ           58 // (1 << 4)
#define XPCS__IRQ               59 // (1 << 5)
#define XGMAC_GIC_IRQ		74
#define XGMAC_TX0_GIC_IRQ       75
#define XGMAC_TX1_GIC_IRQ       76
#define XGMAC_RX0_GIC_IRQ       77
#define XGMAC_RX1_GIC_IRQ       78
#define XPCS_GIC_IRQ            79

/* DDR IRQ */
#define DDR_GIC_IRQ_0           96
#define DDR_GIC_IRQ_1           97
#define DDR_GIC_IRQ_2           98
#define DDR_IRQ_0               (1 << 0)
#define DDR_IRQ_1               (1 << 1)
#define DDR_IRQ_2               (1 << 2)

/* DMAC IRQ */
#define CRYPTO_DMAC_GIC_IRQ	104
#define CRYPTO_DMAC_IRQ		(1 << 1)

/* AXI */
#define AXI_CPU_IRQ			63  /* AXI */
#define AXI_GIC_IRQ			127 /* AXI */

#endif /* __IRQ_MAP_MIPS_H__ */
