#include <asm/arch/sysreg.h>
#include <asm/arch/regmacro.S>

#define ASM		// This definition is strictly needed before 
#include <preparemmu.h>	// including special headers to avoid compilation error
#include <asm/dbgout.h>

#ifdef DEBUG

#define DBG(...) __VA_ARGS__

#else // DEBUG not defined

#define DBG(...)

#endif

.global prepare_mmu

.text

prepare_mmu:

	savereg
	
	lui t0, 0xDEAD
	ori t0, t0, 0xBEAF
	
	DBG(dbg_out TREG, t0)

	and t0, t0, zero		// Set virtual address as 0x0

	lui t1, (0x20000000 >> 16)	// Load upper 16 bit of physical address
	ori t1, (0x20000000 & 0xFFFF)	// Load lower 16 bit of physical address

	addi t2, zero, 0x3		// TLBEntry index

	addi t3, zero, 0x1		// Set page size
	sll t3, t3, 28			// 1 << 28 = 256 MB

	// PageMask = 0x1FFFE000 is for 256MB pages
	lui t4, 0x1FFF			// Load upper 16bit of pagemask
	ori t4, t4, 0xE000		// Load lower 16bit of pagemask
	mtc0 t4, MIPS_CP0_TLB_PG_MASK
	ehb
	sync
1:
	bltz t2, 2f			// Exit from cycle if index < 0
	nop

	mtc0 t2, MIPS_CP0_TLB_INDEX
	ehb
	sync

	addi t4, zero, 20
	srl t4, t1, t4			// Load PFN[29:6] =  PhysAddr >> 20
	ori t4, t4, 0x13		// Add C[5:3]=2, D[2]=0, V[1]=1, G[0]=1 bits 	
	mtc0 t4, MIPS_CP0_TLB_LO0
	ehb
	sync

	add t1, t1, t3
	addi t4, zero, 20
	srl t4, t1, t4			// Load PFN[29:6] =  PhysAddr >> 20
	ori t4, t4, 0x13		// Add C[5:3]=2, D[2]=0, V[1]=1, G[0]=1 bits 	
	mtc0 t4, MIPS_CP0_TLB_LO1
	ehb
	sync

	mtc0 t0, MIPS_CP0_TLB_ENTRY_HI
	ehb
	sync

	add t0, t0, t3			// Increment virtual address
	add t1, t1, t3			// Increment physical address
	b 1b				// Go to next iteration
	addi t2, t2, -0x1		// Decrement 

2:
	restorereg
	jr	ra
	addi	v0, zero, 0xA5
