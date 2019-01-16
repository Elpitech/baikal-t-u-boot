.macro savereg
//	addiu sp, sp, -44
	subu sp, sp, 44
	sw ra, 0x0(sp)
	sw t0, 0x4(sp)
	sw t1, 0x8(sp)
	sw t2, 0xC(sp)
	sw t3, 0x10(sp)
	sw t4, 0x14(sp)
	sw t5, 0x18(sp)
	sw t6, 0x1C(sp)
	sw t7, 0x20(sp)
	sw t8, 0x24(sp)
	sw t9, 0x28(sp)

.endm

.macro restorereg
	lw ra, 0x0(sp)
	lw t0, 0x4(sp)
	lw t1, 0x8(sp)
	lw t2, 0xC(sp)
	lw t3, 0x10(sp)
	lw t4, 0x14(sp)
	lw t5, 0x18(sp)
	lw t6, 0x1C(sp)
	lw t7, 0x20(sp)
	lw t8, 0x24(sp)
	lw t9, 0x28(sp)
	addiu sp, sp, 44
.endm

.macro _savereg addr
	la a0, \addr
        sw ra, 0x0(a0)
	sw t0, 0x4(a0)
	sw t1, 0x8(a0)
	sw t2, 0xC(a0)
	sw t3, 0x10(a0)
	sw t4, 0x14(a0)
	sw t5, 0x18(a0)
	sw t6, 0x1C(a0)
	sw t7, 0x20(a0)
	sw t8, 0x24(a0)
	sw t9, 0x28(a0)

.endm

.macro _restorereg addr
	la a0, \addr
	lw ra, 0x0(a0)
	lw t0, 0x4(a0)
	lw t1, 0x8(a0)
	lw t2, 0xC(a0)
	lw t3, 0x10(a0)
	lw t4, 0x14(a0)
	lw t5, 0x18(a0)
	lw t6, 0x1C(a0)
	lw t7, 0x20(a0)
	lw t8, 0x24(a0)
	lw t9, 0x28(a0)
.endm
