#include <common.h>
#include <asm/dbgout.h>

void debug_out(int type, int val) {
	printf("DBG: %s()\n", __func__);
	if(type & TYPEMASK != TYPEMAGIC) {
		printf("DBG_OUT: Type:0x%X, Value:0x%X\n", type, val);
		return;
	}

	if(type == TREG) {
		printf("DBG_OUT: t register value:0x%X\n", val);
		return;
	}
	if(type == CFG0) {
		printf("DBG_OUT: Config register 0 value:0x%08X\n", val);
		return;
	}
	if(type == CFG1) {
		printf("DBG_OUT: Config register 1 value:0x%08X\n", val);
		return;
	}
	if(type == CFG2) {
		printf("DBG_OUT: Config register 2 value:0x%08X\n", val);
		return;
	}
	if(type == CFG3) {
		printf("DBG_OUT: Config register 3 value:0x%08X\n", val);
		return;
	}
	if(type == CFG4) {
		printf("DBG_OUT: Config register 4 value:0x%08X\n", val);
		return;
	}
	if(type == CFG5) {
		printf("DBG_OUT: Config register 5 value:0x%08X\n", val);
		return;
	}
	if(type == CFG6) {
		printf("DBG_OUT: Config register 6 value:0x%08X\n", val);
		return;
	}

}
