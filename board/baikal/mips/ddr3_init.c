#include <stdint.h>
#include <stdbool.h>
#include <config.h>
#include "llenv_spd.h"
#include <linux/string.h>


int baikal_read_spd (uint32_t addr, int alen, uint8_t *buffer, int len);

uint8_t  *ddr_buffer0 = (uint8_t  *) CONFIG_SRAM_BUF0_BASE;
uint32_t *ddr_buffer1 = (uint32_t *) CONFIG_SRAM_BUF1_BASE;

#define LLENV_SET_SPD_REG(r,v)  ddr_buffer1[(r)]=(v)
#define read_spd(x) ddr_buffer0[x]





#define DLL_FAST_EXIT 0
#define MEMC_FREQ_RATIO 2
#define MEMC_MODE_2T 0
#define BL 8


#define DDR3_TCK_SPD		read_spd(12)
#define DDR3_TCK_1066		0xF
#define DDR3_TCK_1333		0xC
#define DDR3_TCK_1600		0xA

#define DDR3_TAA_SPD		read_spd(16)
#define DDR3_TAA_1066F		0x69
#define DDR3_TAA_1333H		0x6C
#define DDR3_TAA_1600K		0x6E

#define DDR3_TRCD_SPD		read_spd(18)
#define DDR3_TRCD_1066F		0x69
#define DDR3_TRCD_1333H		0x6C
#define DDR3_TRCD_1600K		0x6E

#define DDR3_TRRD_SPD		read_spd(19)
#define DDR3_TRRD_MIN_6NS	0x30
#define DDR3_TRRD_MIN_7_5NS	0x3c
#define DDR3_TRRD_MIN_10NS	0x50

#define DDR3_TPR_SPD		read_spd(20)
#define DDR3_TPR_1066F		0x69
#define DDR3_TPR_1333H		0x6c
#define DDR3_TPR_1600K		0x6E

#define DDR3_TRAS_SPD		read_spd(22)
#define DDR3_TRAS_1066F		0x2C
#define DDR3_TRAS_1333H		0x20
#define DDR3_TRAS_1600K		0x18

#define DDR3_TRC_SPD		read_spd(23)
#define DDR3_TRC_1066F		0x95
#define DDR3_TRC_1333H		0x8C
#define DDR3_TRC_1600K		0x86

#define DDR3_TFAW_H_SPD		read_spd(28)
#define DDR3_TFAW_H_1066	0x1
#define DDR3_TFAW_H_1333	0x1
#define DDR3_TFAW_H_1600	0x1

#define DDR3_TFAW_SPD		read_spd(29)
#define DDR3_TFAW_1066		0x90
#define DDR3_TFAW_1333		0x68
#define DDR3_TFAW_1600		0x40

#if defined(DDR_SPEED_BIN_1600)
#define DDR3_TCK	DDR3_TCK_1600
#define DDR3_TAA	DDR3_TAA_1600K
#define DDR3_TRCD	DDR3_TRCD_1600K
#define DDR3_TRRD	DDR3_TRRD_MIN_7_5NS
#define DDR3_TPR	DDR3_TPR_1600K
#define DDR3_TRAS	DDR3_TRAS_1600K
#define DDR3_TRC	DDR3_TRC_1600K
#define DDR3_TFAW_H	DDR3_TFAW_H_1600
#define DDR3_TFAW	DDR3_TFAW_1600
#elif defined(DDR_SPEED_BIN_1333)
#define DDR3_TCK	DDR3_TCK_1333
#define DDR3_TAA	DDR3_TAA_1333H
#define DDR3_TRCD	DDR3_TRCD_1333H
#define DDR3_TRRD	DDR3_TRRD_MIN_7_5NS
#define DDR3_TPR	DDR3_TPR_1333H
#define DDR3_TRAS	DDR3_TRAS_1333H
#define DDR3_TRC	DDR3_TRC_1333H
#define DDR3_TFAW_H	DDR3_TFAW_H_1333
#define DDR3_TFAW	DDR3_TFAW_1333
#elif defined(DDR_SPEED_BIN_1066)
#define DDR3_TCK	DDR3_TCK_1066
#define DDR3_TAA	DDR3_TAA_1066F
#define DDR3_TRCD	DDR3_TRCD_1066F
#define DDR3_TRRD	DDR3_TRRD_MIN_10NS
#define DDR3_TPR	DDR3_TPR_1066F
#define DDR3_TRAS	DDR3_TRAS_1066F
#define DDR3_TRC	DDR3_TRC_1066F
#define DDR3_TFAW_H	DDR3_TFAW_H_1066
#define DDR3_TFAW	DDR3_TFAW_1066
#elif defined(DDR_SPEED_BIN_SPD)
#define DDR3_TCK	DDR3_TCK_SPD
#define DDR3_TAA	DDR3_TAA_SPD
#define DDR3_TRCD	DDR3_TRCD_SPD
#define DDR3_TRRD	DDR3_TRRD_SPD
#define DDR3_TPR	DDR3_TPR_SPD
#define DDR3_TRAS	DDR3_TRAS_SPD
#define DDR3_TRC	DDR3_TRC_SPD
#define DDR3_TFAW_H	DDR3_TFAW_H_SPD
#define DDR3_TFAW	DDR3_TFAW_SPD
#else
#error "Time setting does not declared."
#endif


struct ddr3_timings {
	unsigned tCK;
	unsigned tAA;
	unsigned tRCD;
	unsigned tRP;
	unsigned tRC;
	unsigned tRASmin;
	unsigned tFAWmin;
	unsigned CL;
};

static unsigned roundup(unsigned x, unsigned y)
{
	return (x - 1) / y + 1;
}

static unsigned max(unsigned a, unsigned b)
{
	return (a > b) ? a : b;
}

static unsigned encode_wr(unsigned wr)
{
    if ((wr >= 5) && (wr <= 7)) {
        return (wr - 4);
    } else if (((wr % 2) == 0) && (wr >= 8) && (wr <= 14)) {
        return wr / 2;
    }
    return 0;
}

static unsigned encode_cl(unsigned cl)
{
	if (cl < 5)
		return 0;
	if (cl < 12)
		return (cl-4) << 1;

	return ((cl - 12) << 1) | 1;
}

static unsigned time2memc(unsigned t, int up)
{
	if (up)
		t = roundup(t, MEMC_FREQ_RATIO);
	else
		t /= MEMC_FREQ_RATIO;
	t += MEMC_MODE_2T;
	return t;
}

#define DDR3_MR0_PD_S	12
#define DDR3_MR0_WR_S	9
#define DDR3_MR0_CL_S0	2
#define DDR3_MR0_CL_S1	4

#define DDR3_MR1_AL_S	3
#define DDR3_MR1_AL_0	0
#define DDR3_MR1_AL_CL1	1
#define DDR3_MR1_AL_CL2	2
#if defined(CONFIG_BAIKAL_BFK)
#define DDR3_MR1_AL	DDR3_MR1_AL_CL1
#else
#define DDR3_MR1_AL	DDR3_MR1_AL_0
#endif

#define DDR3_MR1_DR_S   1
#define DDR3_MR1_RTT_S  2


#define DDR3_MR2_CWL_S	3
#define DDR3_MR2_RTT_S  9


/*
const uint8_t ddr_user_spd [256] = {
    0x91, 0x10, 0x0b, 0x02, 0x04, 0x21, 0x02, 0x01, 0x02, 0x55, 0x01, 0x08, 0x0a, 0x00, 0xac, 0x00,
    0x6e, 0x78, 0x6e, 0x3c, 0x6e, 0x11, 0x18, 0x86, 0x20, 0x08, 0x3c, 0x3c, 0x00, 0xf0, 0x83, 0x8f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x05,
};
*/


/* BFK 1.6 */
#if defined(DDR_USER_SPD)
# if defined(DDR_USER_SPD_MT41K2G4)
const uint8_t ddr_user_spd [256] = {
    0x91, 0x10, 0x0b, 0x02, 0x04, 0x22, 0x02, 0x08, 0x02, 0x55, 0x01, 0x08, 0x0a, 0x00, 0xac, 0x00,
    0x6e, 0x78, 0x6e, 0x3c, 0x6e, 0x11, 0x18, 0x86, 0x20, 0x08, 0x3c, 0x3c, 0x00, 0xf0, 0x83, 0x8f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0xad,
};
# elif defined(DDR_USER_SPD_MT41K512M16)
const uint8_t ddr_user_spd [256] = {
    0x91, 0x10, 0x0b, 0x02, 0x05, 0x21, 0x02, 0x02, 0x02, 0x55, 0x01, 0x08, 0x0a, 0x00, 0xac, 0x00,
    0x6e, 0x78, 0x6e, 0x3c, 0x6e, 0x11, 0x18, 0x86, 0xf0, 0x0a, 0x3c, 0x3c, 0x00, 0xf0, 0x83, 0x8f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbe, 0x0b,
};
# elif defined(DDR_USER_SPD_MT41K1G8)
const uint8_t ddr_user_spd [256] = {
    0x91, 0x10, 0x0b, 0x02, 0x04, 0x22, 0x02, 0x01, 0x02, 0x55, 0x01, 0x08, 0x0a, 0x00, 0xac, 0x00,
    0x6e, 0x78, 0x6e, 0x3c, 0x6e, 0x11, 0x18, 0x86, 0x20, 0x08, 0x3c, 0x3c, 0x00, 0xf0, 0x83, 0x8f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7b, 0xe2,
};
# endif
#endif

const uint32_t ddr_user_regs [64] = {
    0x1, 0x2, 0x3, 0x4, 0x5
};


static uint16_t crc16 (int count)
{
    unsigned crc = 0, i, j;
    for (j = 0; j < count; ++j) {
        crc ^= read_spd(j) << 8;
        for (i = 0; i < 8; ++i) {
            unsigned next = crc << 1;
            if (crc & 0x8000)
                next ^= 0x1021;
            crc = next;
        }
    }
    return crc;
}


int llenv_prepare_buffer0 (void)
{
    memset(ddr_buffer0, 0, 256);
    memset(ddr_buffer1, 0, 256);

#if defined(DDR_USER_SPD)
    memcpy(ddr_buffer0, ddr_user_spd, 256);
    return 0;
#else /* DDR_USER_SPD */
    return baikal_read_spd(0, 1, ddr_buffer0, 256);
#endif /* DDR_USER_SPD */
}

int llenv_prepare_buffer1 (void)
{
#ifdef DDR_USER_REGS
    memcpy(ddr_buffer1, ddr_user_regs, 256);
    return 0;

#else /* DDR_USER_REGS */

    // all times in ps
    // ddr3 common
    int i;
    unsigned tREFI = 7800000; //(less 85C)
    unsigned cov = (read_spd(0) & (1U << 7)) ? 116 : 125;

    unsigned crc = crc16(cov + 1);
    if (crc != ((read_spd(127) << 8) | read_spd(126)))
	   return -1;

#if defined(CONFIG_BAIKAL_BFK)
    unsigned bus_width = 8 << (read_spd(8) & 7);
#else
    unsigned bus_width = 32; // bus width is always 32
#endif
    unsigned chip_width = 4 << (read_spd(7) & 7);
    unsigned ranks = 1 + ((read_spd(7) >> 3) & 7);
    unsigned whole_mem = (256 << (read_spd(4) & 0xf)) * bus_width/chip_width/8*ranks;
    unsigned cols = 9 + (read_spd(5) & 7);
    unsigned rows = 12 + (read_spd(5) >> 3);
    struct ddr3_timings spd_t;
    unsigned dv = 1000 * read_spd(10) / read_spd(11);
    spd_t.tCK = DDR3_TCK * dv; /* read_spd(12) */
    spd_t.tAA = DDR3_TAA * dv; /* read_spd(16) */
    unsigned tWRmin = read_spd(17) * dv;
    spd_t.tRCD = DDR3_TRCD * dv; /* read_spd(18) */
    unsigned tRRD = DDR3_TRRD * dv; /* read_spd(19) */


    spd_t.tRP = DDR3_TPR * dv; /* read_spd(20) */
    spd_t.tRC = (((read_spd(21) >> 4) << 8) | DDR3_TRC) * dv; /* read_spd(23) */
    spd_t.tRASmin = (((0xf & read_spd(21)) << 8) | DDR3_TRAS) * dv; /* read_spd(22) */
    unsigned xtRFC = ((read_spd(25) << 8) | read_spd(24)) * dv;
    unsigned tWTR = read_spd(26) * dv;
    spd_t.tFAWmin = ((DDR3_TFAW_H << 8) | DDR3_TFAW) * dv; /* read_spd(28 & 29) */
    spd_t.CL = spd_t.tAA/spd_t.tCK;

    uint32_t t1, reg;
    uint32_t mr0, mr1, mr2;


    unsigned AL = 0;
    // MR0
    mr0 = (encode_wr(tWRmin/spd_t.tCK) << DDR3_MR0_WR_S) |
    ((encode_cl(spd_t.CL) & 0xe) << (DDR3_MR0_CL_S1 - 1)) |
    ((encode_cl(spd_t.CL) & 1) << DDR3_MR0_CL_S0);
    // MR1
    mr1 = DDR3_MR1_AL << DDR3_MR1_AL_S;
    mr1 |= 1 << DDR3_MR1_DR_S; // 34 Ohm
    mr1 |= 1 << DDR3_MR1_RTT_S; // 60 Ohm

    if (DDR3_MR1_AL == DDR3_MR1_AL_CL1) {
        AL = spd_t.CL - 1;
    } else if (DDR3_MR1_AL == DDR3_MR1_AL_CL2) {
        AL = spd_t.CL - 2;
    }

    if (DLL_FAST_EXIT)
	    mr1 |= 1 << DDR3_MR0_PD_S;
    reg = (mr0 << 16) | mr1;
    LLENV_SET_SPD_REG(DDR3_SPD_INIT3, reg);


    // MR2
    unsigned CWL = spd_t.CL - 1;
    for (i = 0; i < 3; ++i) {
	    if (spd_t.CL > 7 + i * 2)
		    --CWL;
    }


    mr2 = (CWL - 5) << DDR3_MR2_CWL_S;
#if defined(CONFIG_BAIKAL_BFK)
    // ...
#else
    mr2 |= 1 << DDR3_MR2_RTT_S; // 60 Ohm
#endif
    reg = mr2 << 16;
    LLENV_SET_SPD_REG(DDR3_SPD_INIT4, reg);

#if defined(CONFIG_BAIKAL_BFK)
    reg = 0x02918210;
#else
    reg = 0x02808200 | /* rddata_en */ ((spd_t.CL + AL - 4) << 16) | /* wrlat */  (CWL + AL - 2);
#endif
    LLENV_SET_SPD_REG(DDR3_SPD_DFITMG0, reg);


    reg = 0x00080404;
    LLENV_SET_SPD_REG(DDR3_SPD_DFITMG1, reg);


    reg = 0;
    //wr2pre = WL + BL/2 + tWRmin
    // WL = AL + CWL
    t1 = time2memc(CWL + AL + BL/2 + tWRmin/spd_t.tCK, false);
    reg |= t1 << 24;
    // t_faw
    t1 = time2memc(spd_t.tFAWmin/spd_t.tCK, true);
    reg |= t1 << 16;
    // t_ras_max = tRASmax/1024 = (tREFI * 9)/1024
    t1 = time2memc(((tREFI * 9) / spd_t.tCK - 1) / 1024, false);
#if defined(CONFIG_BAIKAL_BFK)
    // ...
#else
    if (MEMC_FREQ_RATIO == 2) {
        t1 = (t1 - 1) / 2;
    }
#endif
    reg |= t1 << 8;
    // t_ras_min
    t1 = time2memc(spd_t.tRASmin / spd_t.tCK, false);
#if defined(CONFIG_BAIKAL_BFK)
    // ...
#else
    if (MEMC_FREQ_RATIO == 2) {
        t1 = (t1) / 2;
    }
#endif
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG0, reg);


    reg = 0;
    // t_xp
    t1 = (DLL_FAST_EXIT)?(max(3 * spd_t.tCK, (spd_t.tCK < 1875) ? 6000 : 7500)) : max(10 * spd_t.tCK, 24000);
    t1 = time2memc(t1 / spd_t.tCK, true);
    reg |= t1 << 16;
    // rd2pre: AL + max(4CK, 7.5ns)
    t1 = max(4 * spd_t.tCK, 7500);
    t1 = time2memc((t1 / spd_t.tCK) + AL, false);
    reg |= t1 << 8;
    // t_rc
    t1 = time2memc(spd_t.tRC/spd_t.tCK, true);
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG1, reg);


    reg = 0;
    // write_latency
    // t1 = time2memc(CWL + AL, true);
    // reg |= t1 << 24;
    // read_latency
    // t1 = time2memc(spd_t.CL + AL, true);
    // reg |= t1 << 16;
    // rd2wr = RL + BL/2 + 2 - WL
    t1 = time2memc(spd_t.CL + BL/2 + 2 - CWL, true);
    reg |= t1 << 8;
    // wr2rd = CWL + BL/2 + tWTR
    t1 = max(4 * spd_t.tCK, 7500);
    t1 = time2memc(t1 / spd_t.tCK + CWL + BL/2, true);
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG2, reg);


    reg = 0;
    // t_mrd
    t1 = time2memc(4, true);
    reg |= t1 << 12;
    // t_mod
    t1 = max(12 * spd_t.tCK, 15000);
    t1 = time2memc(t1/spd_t.tCK, true);
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG3, reg);


    reg = 0;
    // t_rcd
    t1 = time2memc(spd_t.tRCD/spd_t.tCK - AL, true);
    reg |= t1 << 24;
    // t_ccd
    t1 = time2memc(4, true);
    reg |= t1 << 16;
    // t_rrd
    t1 = time2memc(tRRD/spd_t.tCK, true);
    reg |= t1 << 8;
    // t_rp
#if defined(CONFIG_BAIKAL_BFK)
    t1 = time2memc(spd_t.tRP/spd_t.tCK, true) + MEMC_FREQ_RATIO - 1;
#else
    t1 = time2memc(spd_t.tRP/spd_t.tCK, true);
#endif
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG4, reg);


    reg = 0;
    // t_cksrx
    t1 = max(5 * spd_t.tCK, 10000);
    t1 = time2memc(t1 / spd_t.tCK, true);
    reg |= t1 << 24;
    // t_cksre
    reg |= t1 << 16;
    // t_ckesr
    t1 = max(3 * spd_t.tCK, (spd_t.tCK < 1500) ? 5000 : 5625);
    t1 = time2memc(t1 / spd_t.tCK + 1, true);
    reg |= t1 << 8;
    // t_cke
    t1 = max(3 * spd_t.tCK, (spd_t.tCK < 1500) ? 5000 : 5625);
    t1 = time2memc(t1 / spd_t.tCK, true);
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG5, reg);


    reg = 0;
    // t_xs_dll_x32 = tDDLKmin
    t1 = time2memc(512/32, true);
    reg |= (t1 & 0x7f) << 8;
    // t_xs_x32 = tXSmin
    t1 = max(5 * spd_t.tCK, xtRFC + 10000);
#if defined(CONFIG_BAIKAL_BFK)
    t1 = time2memc(t1 / 32 / spd_t.tCK, true);
#else
    t1 = time2memc(t1 / 32 / spd_t.tCK, true) + 1;
#endif
    reg |= t1 & 0x7f;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG8, reg);


    // t_rfc_nom_x32 = tREFI
#if defined(CONFIG_BAIKAL_BFK)
    t1 = time2memc((tREFI-xtRFC)/spd_t.tCK/32, false);
#else
    t1 = time2memc((tREFI/*-xtRFC*/)/spd_t.tCK/32, false);
#endif
    reg = t1 << 16;
    t1 = time2memc(xtRFC/spd_t.tCK, true);
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_RFSHTMG, reg);
    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP2, 0);


    reg = 0x0f000000U;
    if (cols >= 10)
	    reg = 0;
    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP3, reg);


    reg = 0x00000f0fU;
#if defined(CONFIG_BAIKAL_BFK)
    if (cols == 12) {
        reg &= ~0xfU;
    }
#else
    if (cols == 11)
	    reg &= ~0xfU;
    else if (cols == 12)
	    reg = 0;
#endif
    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP4, reg);

#if defined(CONFIG_BAIKAL_BFK)
    unsigned row_offset = cols - 7;
#else
#define ROW_INTERNAL_BASE	6
    unsigned row_offset = cols - ROW_INTERNAL_BASE;
#endif
    reg = 0;
    for (i = 0; i < 4; ++i)
	    reg |= row_offset << (i * 8);
    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP5, reg);
    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP6, reg);

#if defined(CONFIG_BAIKAL_BFK)
    unsigned bank_offset = row_offset + rows + 6 - 2;
#else
#define BANK_INTERNAL_BASE	2
    unsigned bank_offset = cols + rows - BANK_INTERNAL_BASE;
#endif
    unsigned banks = 3;
    reg = 0;
    for (i = 0; i < banks; ++i)
	    reg |= bank_offset << (i * 8);
    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP1, reg);

#if defined(CONFIG_BAIKAL_BFK)
    unsigned rank_offset = bank_offset - 3 + 2;
    reg = 0;
    for (i = 0; i < ranks; ++i)
        reg |= rank_offset << (i * 8);
#else
    #define RANK_INTERNAL_BASE	6
    unsigned rank_offset = cols + rows + banks - RANK_INTERNAL_BASE;
    if (ranks == 1) reg = 0x1f;
    else reg = rank_offset;
#endif
    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP0, reg);
    LLENV_SET_SPD_REG(DDR3_SPD_SARBASE0, 0);
    LLENV_SET_SPD_REG(DDR3_SPD_SARBASE1, 0x2);

    reg = (whole_mem / 256) + 2;
    LLENV_SET_SPD_REG(DDR3_SPD_SARBASE2, reg);
    LLENV_SET_SPD_REG(DDR3_SPD_SARSIZE0, 0);

    reg = (whole_mem / 256 - 1 - 1);
    LLENV_SET_SPD_REG(DDR3_SPD_SARSIZE1, reg);
    LLENV_SET_SPD_REG(DDR3_SPD_SARSIZE2, 0);


    // tDINIT0 = 500uS/tCK;
    t1 = (500 * 1000 * 1000)/spd_t.tCK;
    reg = t1 & 0xfffff;
    // tDINIT1 = max(tRFC + 10ns,  5*tCK);
    t1 = (max(xtRFC + 10 * 1000, 5 * spd_t.tCK))/spd_t.tCK;
    reg |= t1 << 20;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PTR3, reg);


    // tDINIT2 = 200uS
    t1 = (200*1000*1000)/spd_t.tCK;
    reg = t1 & 0x3ffff;
    // tDINIT3 = 1uS
    t1 = (1*1000*1000)/spd_t.tCK;
    reg |= t1 << 18;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PTR4, reg);


    LLENV_SET_SPD_REG(DDR_PUB_SPD_MR0, mr0);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_MR1, mr1);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_MR2, mr2);

    /*
    ZQCTL0  = 0x008C0040
    ECCCFG0 = 0x00000004
    INIT0   = 0x40020001
    DFIUPD0 = 0x80400003
    MSTR    = 0x03040001
    PGCR1   = 0x020046a0
    PGCR2   = 0x00f016d0
    */

    // tRTP = max (4CK or 7.5ns)
    t1 = max(4*spd_t.tCK, 7500) / spd_t.tCK;
    reg = 0xf & t1;
    // tRP
    t1 = spd_t.tRP/spd_t.tCK;
    reg |= (0x7f & t1) << 8;
    // tRASmin
    t1 = spd_t.tRASmin/spd_t.tCK;
    reg |= (0x7f & t1) << 16;
    // tRRD
    t1 = tRRD/spd_t.tCK;
    reg |= (0xf & t1) << 24;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTPR0, reg);

    //tMRD
    t1 = 4;
    reg = (t1 - 4) & 3;
    //tMOD
    t1 = max(12 * spd_t.tCK, 15000) / spd_t.tCK;
    reg |= (7 & (t1 - 12)) << 8;
    //tFAWmin
    t1 = spd_t.tFAWmin / spd_t.tCK;
    reg |= (0x3f & t1) << 16;
    //tWLMRD
    t1 = 40;
    reg |= t1 << 24;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTPR1, reg);

    //tXS
    t1 = max(512, max(5 * spd_t.tCK, xtRFC + 10000) / spd_t.tCK);
    reg = 0x3ff & t1;
    // t_ckesr
    t1 = max(3 * spd_t.tCK, (spd_t.tCK < 1500) ? 5000 : 5625) / spd_t.tCK + 1;
    reg |= (0xf & t1) << 16;
    // tRODT = tRTW = 0;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTPR2, reg);

    //tDQSCK -- LPDDR3 only
    reg = 1 | (1 << 8);
    // tDLLK
    t1 = 512;
    reg |= t1 << 16;
    //tCCD = BL, 0 encoding
    //tOFD = 0
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTPR3, reg);

    //tXP
    t1 = (DLL_FAST_EXIT)?
	(max(3 * spd_t.tCK, (spd_t.tCK < 1875) ? 6000 : 7500)) :
	max(10 * spd_t.tCK, 24000) / spd_t.tCK;
    reg = 0x1f & t1;
    //tWLO
    t1 = ((spd_t.tCK < 1250) ? 9000 : 7500) / spd_t.tCK;
    reg |= (0xf & t1) << 8;
    //tRFC
    t1 = xtRFC/spd_t.tCK;
    reg |= (0x3ff & t1) << 16;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTPR4, reg);

    //tWTR
    t1 = tWTR/spd_t.tCK;
    reg = 0x1f & t1;
    //tRCD
    t1 = spd_t.tRCD/spd_t.tCK;
    reg |= (0x3f & t1) << 8;
    //tRC
    t1 = spd_t.tRC/spd_t.tCK;
    reg |= (0xff & t1) << 16;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTPR5, reg);

#if defined(CONFIG_BAIKAL_BFK)
    LLENV_SET_SPD_REG(DDR3_SPD_ODTCFG, 0x0600060C);
#else
    reg = (0x06000600 | ((spd_t.CL - CWL) << 2));
    LLENV_SET_SPD_REG(DDR3_SPD_ODTCFG, reg);
#endif

#if defined(CONFIG_BAIKAL_BFK)
    LLENV_SET_SPD_REG(DDR3_SPD_ODTMAP, 0x00000201);
#else
    if (ranks == 1)   reg = 0x00000001;
    else              reg = 0x00000201;
    LLENV_SET_SPD_REG(DDR3_SPD_ODTMAP, reg);
#endif

    LLENV_SET_SPD_REG(DDR3_SPD_ZQCTL0, 0x008C0040);
#ifdef ECC_DDR_ENABLE
    reg = 0x00000004;
#else
    reg = 0;
#endif /* ECC_DDR_ENABLE */
    LLENV_SET_SPD_REG(DDR3_SPD_ECCCFG0, reg);
    LLENV_SET_SPD_REG(DDR3_SPD_INIT0, 0x40020001);
    LLENV_SET_SPD_REG(DDR3_SPD_DFIUPD0, 0x80400003);
#if defined(CONFIG_BAIKAL_BFK)
    // ...
#else
    LLENV_SET_SPD_REG(DDR3_SPD_INIT1, 0x00010000);
#endif

#if defined(CONFIG_BAIKAL_BFK)
    LLENV_SET_SPD_REG(DDR3_SPD_MSTR, 0x01040001);  //0x03040001 - if not single rank
#else
    if (ranks == 1)  rank_offset = 0x1;
    else             rank_offset = 0x3;
    LLENV_SET_SPD_REG(DDR3_SPD_MSTR,    (0x40001 | (rank_offset << 24)));
    LLENV_SET_SPD_REG(DDR3_SPD_MRCTRL0, (rank_offset << 4));
#endif
#if defined(CONFIG_BAIKAL_BFK)
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTCR1, 0x00010237);
#else
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTCR1, (0x237 | (rank_offset << 16)));
#endif
    LLENV_SET_SPD_REG(DDR3_SPD_PCTRL_0, 0x00000001);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PGCR1, 0x020046a0);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PTR1, 0xd05612c0);

    reg = tREFI * 9 / spd_t.tCK - 400;
    reg |= (0xf << 20);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PGCR2, reg);

    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTCR0, 0x800031c7);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PIR_VAL1, 0x00000073);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DXCCR, 0x20401004);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PIR_VAL2, 0x0000ff81);
    return 0;

#endif  /* DDR_USER_REGS */
}
