.macro savereg
        addi sp, sp, -0x4
        sw ra, 0x0(sp)
.endm

.macro restorereg
        lw ra, 0x0(sp)
        addi sp, sp, 0x4
.endm
