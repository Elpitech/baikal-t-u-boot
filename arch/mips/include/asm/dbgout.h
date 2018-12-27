#ifndef DEBUG_OUT
#define DEBUG_OUT
#ifndef ASM
// Place here definitions for C-code

void cfgreg_out(void);
void ddr_ddrc_regs_print(void);

#else // ASM defined
// Place here definitions for asm-code

.macro	dbg_out type, val
	
	lui a0, (\type >> 16)
	ori a0, a0,(\type & 0x0000FFFF)
	add a1, zero, \val
	bal debug_out
.endm



#endif // ASM
// Place here common definitions

#define TYPEMAGIC	0xDEAD0000
#define TYPEMASK	0xFFFF0000
#define TREG		0xDEADFFFF
#define CFG0		0xDEAD0CF0
#define CFG1		0xDEAD0CF1
#define CFG2		0xDEAD0CF2
#define CFG3		0xDEAD0CF3
#define CFG4		0xDEAD0CF4
#define CFG5		0xDEAD0CF5
#define CFG6		0xDEAD0CF6

#endif // DEBUG_OUT
